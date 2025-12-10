## DOC: Persistence and Information Gathering

---

## Scheduled Task (Cron) Syntax

This section explains the format used to schedule a repeating command on Unix-like systems.

* **`*/5 * * * *`** = **Run every five minutes, all day, every day.**
* **`%s`** = A **placeholder** where the C code will insert the **actual program's file path**.
* **`>/dev/null 2>&1`** = **Run the command totally silently**; don't show any normal output or any error messages.
* **`\n`** = **Ends the command line** (the newline character).

---

## Cron Persistence Command Breakdown

This is a **shell pipeline** used to automatically add a new scheduled job for the program to the system's crontab.

* **`crontab -l`** = **Lists** the user's current scheduled jobs.
* **`2>`** = Redirects **error messages** away from the main output stream.
* **`/dev/null`** = A **"black hole"** where messages are sent to be completely forgotten.
* **`|`** = **Pipes** (connects) the output of one command to the input of the next.
* **`{ ... }`** = **Groups** the commands inside to work as a single block.
* **`cat;`** = **Prints** the existing list of scheduled jobs (received via the pipe).
* **`echo "..."`** = **Prints** the new, malicious job command (to be added to the list).
* **`/tmp/.system_daemon`** = The **location of the implant** (the file to run) for the scheduled task.
* **`crontab -`** = **Saves/installs** the full, updated list of scheduled jobs, overwriting the old one.

---

## File Copy Command Breakdown

This explains the shell command used to copy the implant to a new, hidden location, ensuring the copy operation is silent.

* **`cp`** = The standard shell command for **copying files**.
* **`%s`** = A **placeholder** for the **source file's full path**.
* **`/tmp/.system_daemon`** = The **destination path**; a hidden file in the temporary directory.
* **`2>/dev/null`** = Ensures that **any error messages** that occur during the copy process are **silently hidden**.

---

## OS Information Gathering Logic

This C code block attempts to execute a system command to retrieve and safely store details about the host machine's Operating System.

* **`fp = popen("uname -a 2>/dev/null", "r");`**
    * **`popen`** executes the **`uname -a`** (get Unix name and details) command and opens a pipe (`fp`) for **reading (`"r"`)** its output.
    * **`2>/dev/null`** ensures the execution is silent by discarding errors.
* **`if (fp)`**
    * **Checks** if the command successfully started (if the pipe `fp` is open).
* **`fgets(os_info, sizeof(os_info), fp);`**
    * If successful, **`fgets`** **reads the first line of output** (the OS info) from the pipe (`fp`) and safely stores it into the **`os_info`** buffer, preventing overflow.
* **`pclose(fp);`**
    * **Closes the pipe** opened by `popen`, releasing system resources.
* **`else { ... }`**
    * This block executes if the command **failed** to run.
* **`strcpy(os_info, "unknown");`**
    * If there's an error, **`strcpy`** sets the **`os_info`** variable to the default value **`"unknown"`**.