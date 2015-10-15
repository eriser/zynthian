#!/usr/bin/python3

import time
import threading
from pythonosc import dispatcher
from pythonosc import osc_server
from pythonosc import osc_message_builder
from pythonosc import udp_client

ip_local="127.0.0.1"
port_local=5005
ip_remote="127.0.0.1"
port_remote=6699

lo_addr="osc.udp://[::ffff:"+ip_local+"]:"+str(port_local)

dispatcher = dispatcher.Dispatcher()
# This will print all parameters to stdout.
dispatcher.map("/broadcast", print)
dispatcher.map("/echo", print)
dispatcher.map("/paths", print)
dispatcher.map("/loadbank", print)
dispatcher.map("/bank-list", print)
dispatcher.map("/bankview", print)

client = udp_client.UDPClient(ip_remote,port_remote)

#server = ForkingOSCUDPServer((ip_local, port_local), dispatcher)
#server.serve_forever()
server = osc_server.ForkingOSCUDPServer((ip_local,port_local), dispatcher)
server_thread = threading.Thread(target=server.serve_forever)
server_thread.start()

msg = osc_message_builder.OscMessageBuilder(address = "/echo")
msg.add_arg("OSC_URL")
msg.add_arg(lo_addr)
msg = msg.build()
client.send(msg)

time.sleep(0.1)

msg = osc_message_builder.OscMessageBuilder(address = "/loadbank")
msg.add_arg(7)
msg = msg.build()
client.send(msg)

time.sleep(0.1)


server.shutdown()



