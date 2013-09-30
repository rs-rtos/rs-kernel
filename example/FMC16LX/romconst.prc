# Simulator only:
# Copy ROM-mirror area FF4000..FFFFFF to 004000

print "\n\n>Set ROM-mirror memory map...\n"
set map /read H'004000..H'00FFFF
set map /read/code H'FF4000..H'FFffff
show map

print ">Copy ROMCONST for simulation..."
print "OK"
move H'ff4000..H'FFFFFF,H'4000

print "\n-----------------------------------------------------------"
print "\nUse command \"batch prc\\romconst.prc\" after each download"
print "\n-----------------------------------------------------------"
