#!/usr/bin/python

import sys
import alsaseq
import alsamidi

alsaseq.client('ZynthianGUI', 0, 1, True)
alsaseq.connectto( 0, 128, 0 )
alsaseq.connectto( 0, 130, 0 )
alsaseq.connectto( 0, 131, 0 )
alsaseq.start()

# Instrument Select
note=sys.argv[1]
print "Program " + note
event=alsamidi.pgmchangeevent(0, int(note))
alsaseq.output(event)

