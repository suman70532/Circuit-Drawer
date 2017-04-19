# Circuit-Drawer
COP290

Parse top.cir and render them as SVG drawing. Further, display net names in the SVG. Also, make sure the circuit should scale in terms of nodes.

Identify nets - and create them. That is if the same signal is sent to 3 outputs, instead of drawing 3 wires, draw a net that has one source and 3 wire segments branching towards the 3 destinations. Find invalid circuits and flag errors - syntax or semantic errors. Define & enforce design rules.
