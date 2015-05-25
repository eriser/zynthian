#!/usr/bin/python
#------------------------------------------------------------------------------
# midi_autoconnect.py: Daemon-script that autoconnect midi devices.
#                      It connects every input to every ouput.
#------------------------------------------------------------------------------
# Author: Jose Fernando Moyano Dominguez (ZauBeR) 
# Email:  fernando@zauber.es
# Web:    http://zauber.es
# Date:   2015-02-22
#------------------------------------------------------------------------------
# This Software is free. Do whatever you want with it! ;-)
#------------------------------------------------------------------------------

from subprocess import check_output
from subprocess import Popen
from re import findall
from time import sleep

#------------------------------------------------------------------------------

#Refresh time
refresh_time = 2

#Jack Daemon Command
jackd_command = "jackd "

#Synth Module Command
synth_command = "zynaddsubfx --auto-connect --no-gui"
#synth_command = "timidity -iA -Oj"
#synth_command = "fluidsynth /usr/share/soundfonts/FluidR3_GM.sf2"
#synth_command = "setBfree midi.driver midi.upper.channel=2 midi.lower.channel=1"
#bristol_emu = "b3"
#synth_command = "startBristol -" + bristol_emu + " -audio jack -midi alsa"

#------------------------------------------------------------------------------

def midi_autoconnect():
	#Get Midi Devices from aconnect
	midi_inputs=check_output("aconnect -li", shell=True)
	midi_outputs=check_output("aconnect -lo", shell=True)

	#Parse Alsa Midi Client Numbers
	input_clients = findall('client ([\d]+):', midi_inputs)
	output_clients = findall('client ([\d]+):', midi_outputs)

	#Connect every input to every output
	for ic in input_clients:
		for oc in output_clients:
			if (int(ic) > 0 and int(oc) > 0 and int(ic) != 14 and int(oc) != 14 and ic != oc):
				command = "aconnect " + ic + ":0 " + oc + ":0"
				print "Connecting " + ic + " to " + oc + " => " + command
				try:
					check_output(command, shell=True)
				except:
					pass

	return

#------------------------------------------------------------------------------

#Starting Jack Audio
print "Starting Jack Audio => " jackd_command
#check_output(jackd_command, shell=True)

#Run Synth Module Command
print "Running Synth Module => " + synth_command
#Popen(synth_command + " &", shell=True)

#Main loop
while True:
	try:
		midi_autoconnect()
	except:
		pass
	sleep(refresh_time)

#------------------------------------------------------------------------------
