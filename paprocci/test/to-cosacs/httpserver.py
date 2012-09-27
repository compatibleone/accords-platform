from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
from os import curdir, sep
import string,cgi,time
import sys, socket
from datetime import datetime


class Handler(BaseHTTPRequestHandler):

    def do_GET(self):
        try:
            if self.path.endswith("info.html"):   #our dynamic content
                self.send_response(200)
                self.send_header('Content-type',    'text/html')
                self.end_headers()
                self.wfile.write("CompatibleOne Demo. Welcome to " + socket.gethostname())
                self.wfile.write(" (date: " + str(datetime.now()) + ").")
                return
            if self.path.endswith(".html"):
                f = open(curdir + sep + self.path)  # self.path has /test.html
                self.send_response(200)
                self.send_header('Content-type',	'text/html')
                self.end_headers()
                self.wfile.write(f.read())
                f.close()
                return
            return
                
        except IOError:
            self.send_error(404,'File Not Found: %s' % self.path)

def main():
    try:
        server = HTTPServer(('', 8080), Handler)
        print 'Server started...'
        server.serve_forever()
    except KeyboardInterrupt:
        print 'Shutting down server...'
        server.socket.close()

if __name__ == '__main__':
    main()

