#!/usr/bin/python3

import os
import alsaseq
import alsamidi
from tkinter import *

#Instrument Bank Configuration
bank_path="/usr/share/zynaddsubfx/banks"
bank_name="William_Godfrey_Collection"

#Get Preset List from Instrument Bank Dir
presets=[]
for xizfile in sorted(os.listdir(bank_path+"/"+bank_name)):
    if (xizfile[-4:]==".xiz"):
        #print("XIZ: "+xizfile)
        prg=int(xizfile[:4])
        title=xizfile[5:-4]
        if (prg>0 and prg<128):
            presets.append((xizfile,prg,title))


#MIDI Change Program Event when click on preset list
def list_click():
    sel=preset_lb.curselection()
    prg=presets[int(sel[0])][1]
    if (prg>=0 and prg<128):
        print("Change to Preset "+str(prg))
        event=alsamidi.pgmchangeevent(0, prg)
        alsaseq.output(event)

#ALSA MIDI client initialization
alsaseq.client( "ZynthianGUI", 0, 1, True )
#alsaseq.connectto( 0, 130, 0 )
alsaseq.start()

#Tkinter Initialization
root=Tk()
root.minsize(320,240)
root.geometry("320x240")

# Create 3 columns
left=Frame(root, width=72, height=240, bg="blue")
center=Frame(root, width=176, height=240, bg="green")
right=Frame(root, width=72, height=240, bg="blue")
left.grid(row=0, column=0)
center.grid(row=0, column=1)
right.grid(row=0, column=2)

# Title
title=Label(center,
    text="zynthian",
    fg="blue",
    bg="yellow",
    font="Verdana 14 bold")
title.pack(fill="x", expand=True)

# Preset List
preset_fr=Frame(center, width=180, height=220, bg="red")
#preset_fr.pack(fill="both", expand=True)
preset_fr.pack(fill="both")
preset_sb=Scrollbar(preset_fr, orient="vertical")
preset_lb=Listbox(preset_fr,
    width=16,
    height=11,
    font="Verdana 11 normal",
     yscrollcommand=preset_sb.set)
preset_sb.config(command=preset_lb.yview)
preset_lb.pack(side="left", fill="both", expand=True)
preset_lb.pack(side="left")
preset_sb.pack(side="right", fill="y")
for preset in presets:
    preset_lb.insert("end", preset[2])

#preset_lb.bind('<Button-1>', lambda event :list_click() )
preset_lb.bind('<<ListboxSelect>>', lambda event :list_click() )

# Controller Labels
title=Label(left,
    text="control num. 1",
    width=8,
    height=5,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="left")
title.pack(fill="x", expand=True)

title=Label(left,
    text="control num. 2",
    width=8,
    height=6,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="left")
title.pack(fill="x", expand=True)

title=Label(left,
    text="control num. 3",
    width=8,
    height=5,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="left")
title.pack(fill="x", expand=True)

title=Label(right,
    text="control num. 4",
    width=9,
    height=5,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="right")
title.pack(fill="x", expand=True)

title=Label(right,
    text="control num. 5",
    width=9,
    height=6,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="right")
title.pack(fill="x", expand=True)

title=Label(right,
    text="control num. 6",
    width=9,
    height=5,
    fg="yellow",
    bg="blue",
    font="Verdana 9 normal",
    wraplength=72,
    justify="right")
title.pack(fill="x", expand=True)

root.mainloop()
