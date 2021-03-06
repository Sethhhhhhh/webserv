#!/usr/bin/python3
import cgi, sys, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()
sys.path.insert(0, os.getcwd())

message = None

# Test if the file is loaded for the upload
if 'filename' in form:
    fileitem = form['filename']
    fn = os.path.basename(fileitem.filename.replace("\\", "/" ))
    open(fn, 'wb').write(fileitem.file.read())
    message = 'The file "' + fn + '" was uploaded successfully'
else:
    message = 'No file was uploaded'

replyhtml = """
<html>
<body>
<form enctype="multipart/form-data" action="../webserv_cgi" method="post">
<p>File: <input type="file" name="filename" /></p>
<p><input type="submit" value="Upload" name=action/></p>
</form>
<p>%s</p>
</body>
</html>
"""
print(replyhtml % message)