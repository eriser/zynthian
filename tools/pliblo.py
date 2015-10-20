#!/usr/bin/python3

#!/usr/bin/env python

import time
import threading
import sys
import liblo

ip_local="127.0.0.1"
port_local=5005
port_remote=6699

def cb_loadbank(path, args):
	if args[0]:
		print("Bank Selected: %d" % (args[0]))

def cb_bankview(path, args):
	if args[0]:
		print("Instrument %d: %s (%s)" % (args[0],args[1],args[2]))

def cb_volume(path, args):
	if args[0]:
		print("Volume Changed: %d" % (args[0]))

def foo_bar_callback(path, args):
	i, f = args
	print("received message '%s' with arguments '%d' and '%f'" % (path, i, f))

def foo_baz_callback(path, args, types, src, data):
	print("received message '%s'" % path)
	print("blob contains %d bytes, user data was '%s'" % (len(args[0]), data))

def fallback(path, args, types, src):
	print("got unknown message '%s' from '%s'" % (path, src.url))
	for a, t in zip(args, types):
		print("argument of type '%s': %s" % (t, a))
        
def serve_forever(tms=100):
	while True:
		server.recv(tms)

try:
	target = liblo.Address(port_remote)
	server = liblo.Server(port_local)
except liblo.AddressError as err:
	print(err)
	sys.exit()


server.add_method("/bankview", 'iss', cb_bankview)
server.add_method("/loadbank", 'i', cb_loadbank)
server.add_method("/volume", 'i', cb_volume)

# register method taking an int and a float
#server.add_method("/foo/bar", 'if', foo_bar_callback)

# register method taking a blob, and passing user data to the callback
#server.add_method("/foo/baz", 'b', foo_baz_callback, "blah")

# register a fallback for unhandled messages
server.add_method(None, None, fallback)

# loop and dispatch messages every 100ms
server_thread = threading.Thread(target=serve_forever)
server_thread.start()

#lo_addr="osc.udp://[::ffff:"+ip_local+"]:"+str(port_local)
#liblo.send(target, "/echo","OSC_URL",lo_addr)
#time.sleep(0.1)

liblo.send(target, "/loadbank", 7)
time.sleep(0.5)

# send double, int64 and char
#liblo.send(target, "/foo/message2", ('d', 3.1415), ('h', 2**42), ('c', 'x'))

for v in range(0,128):
	liblo.send(target, "/volume", v)
	time.sleep(0.1)

while True:
	time.sleep(0.1)
