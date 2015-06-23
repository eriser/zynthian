#!/usr/bin/python

import time;
import alsaseq
import alsamidi


# An event consists of the following items:
# + The type of the event
# + Event flags. It describes various conditions:
#  + time stamp; "real time" / "song ticks"
#  + time mode; "absolute" / "relative to current time"
# + Timestamp of the event.
# Scheduling queue id.
# Source address of the event, given by the combination of client id and port id numbers.
# Destination address of the event.
# The actual event data. (up to 12 bytes)
#--------------------------------------
# ALSA SEQ MESSAGES:
# (type, flags, tag, queue, time stamp, source, destination, data)
#--------------------------------------
# NOTE ON:
# Status byte : 1001 CCCC
# Data byte 1 : 0PPP PPPP
# Data byte 2 : 0VVV VVVV
#--------------------------------------
# Instrument Select:
# Status byte : 1100 CCCC
# Data byte 1 : 0XXX XXXX
#--------------------------------------
#SND_SEQ_EVENT_SYSTEM
#SND_SEQ_EVENT_RESULT
#SND_SEQ_EVENT_NOTE
#SND_SEQ_EVENT_NOTEON
#SND_SEQ_EVENT_NOTEOFF
#SND_SEQ_EVENT_KEYPRESS
#SND_SEQ_EVENT_CONTROLLER
#SND_SEQ_EVENT_PGMCHANGE
#SND_SEQ_EVENT_CHANPRESS
#SND_SEQ_EVENT_PITCHBEND
#SND_SEQ_EVENT_CONTROL14
#SND_SEQ_EVENT_NONREGPARAM
#SND_SEQ_EVENT_REGPARAM
#SND_SEQ_EVENT_SONGPOS
#SND_SEQ_EVENT_SONGSEL
#SND_SEQ_EVENT_QFRAME
#SND_SEQ_EVENT_TIMESIGN
#SND_SEQ_EVENT_KEYSIGN
#SND_SEQ_EVENT_START
#SND_SEQ_EVENT_CONTINUE
#SND_SEQ_EVENT_STOP
#SND_SEQ_EVENT_SETPOS_TICK
#SND_SEQ_EVENT_SETPOS_TIME
#SND_SEQ_EVENT_TEMPO
#SND_SEQ_EVENT_CLOCK
#SND_SEQ_EVENT_TICK
#SND_SEQ_EVENT_QUEUE_SKEW
#SND_SEQ_EVENT_SYNC_POS
#SND_SEQ_EVENT_TUNE_REQUEST
#SND_SEQ_EVENT_RESET
#SND_SEQ_EVENT_SENSING
#SND_SEQ_EVENT_ECHO
#SND_SEQ_EVENT_OSS
#SND_SEQ_EVENT_CLIENT_START
#SND_SEQ_EVENT_CLIENT_EXIT
#SND_SEQ_EVENT_CLIENT_CHANGE
#SND_SEQ_EVENT_PORT_START
#SND_SEQ_EVENT_PORT_EXIT
#SND_SEQ_EVENT_PORT_CHANGE
#SND_SEQ_EVENT_PORT_SUBSCRIBED
#SND_SEQ_EVENT_PORT_UNSUBSCRIBED
#SND_SEQ_EVENT_USR0-9
#SND_SEQ_EVENT_SYSEX
#SND_SEQ_EVENT_BOUNCE
#SND_SEQ_EVENT_USR_VAR0-4
#SND_SEQ_EVENT_NONE
#SND_SEQ_TIME_STAMP_REAL
#SND_SEQ_QUEUE_DIRECT = 253
#--------------------------------------
# Keyboard Controllers
#
# 91 ??
#--------------------------------------
# ZynAddSubFX Controllers
#
# 1)  Pitch bend 
# 2)  Modullation wheel (Controller number 1) 
# 3)  Volume (no. 7) 
# 4)  Pan (no. 10) 
# 5)  Expression (no. 11) 
# 6)  Sustain (no. 64) 
# 7) Portamento On/Off (no. 65) 
# 8)  Filter Q (no. 71) 
# 9)  Filter cutoff (no. 74) 
# 10) All sound off (no. 120) 
# 11) Reset all controlers (no. 121) 
# 12) All notes off (no. 123) 
#
# ZynAddSubFx has controllers that are not defined in GM.
#
# 1) Bandwidth (Sound control 6) (no. 75) 
#   This increases or decreases the bandwidth of instruments. The default parameter is 64.
#
# 2) Modulation amplitude (Sound control 7) (no. 76) 
#   This decreases the amplitude of modulators on ADsynth. The default parameter is 127.
#
# 3) Resonance Center Frequency (Sound control 8) (no. 77) 
#    This changes the center frequency of the resonance.
#
# 4) Resonance Bandwidth (Sound control 9) (no. 78) 
#    This changes the bandwidth of the resonance. 
#--------------------------------------

alsaseq.client('ZynthianGUI', 0, 1, True)
alsaseq.connectto( 0, 128, 0 )
alsaseq.connectto( 0, 130, 0 )
alsaseq.connectto( 0, 131, 0 )
alsaseq.start()

time.sleep(1)

# Instrument Select
event=alsamidi.pgmchangeevent(0, 0)
alsaseq.output(event)

# Raw Note ON:
alsaseq.output( (6, 1, 0, 0, (0, 0), (0, 0), (0, 0), (0, 70, 127, 0, 100)) )

# Note ON Event
event=alsamidi.noteonevent(0, 66, 120)
alsaseq.output(event)

control=74
for i in (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15):
	value=i*16
	alsaseq.output( (alsaseq.SND_SEQ_EVENT_CONTROLLER, 1, 0, 0, (0, 0), (0, 0), (0, 0), (0, 0, 0, 0, control, value)) )
	time.sleep(0.04)

for i in (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15):
	value=(15-i)*16
	alsaseq.output( (alsaseq.SND_SEQ_EVENT_CONTROLLER, 1, 0, 0, (0, 0), (0, 0), (0, 0), (0, 0, 0, 0, control, value)) )
	time.sleep(0.04)


for i in (0,1,2,3,4,5,6,7,8,9,10,11,12):
	# Note Event
	event=alsamidi.noteonevent(0, 60+i, 127)
	#event=alsamidi.noteevent(0, 60+i, 127, 0, 1900)
	alsaseq.output(event)
	
	time.sleep(2)
	
	# Instrument Select
	event=alsamidi.pgmchangeevent(0, i)
	alsaseq.output(event)
