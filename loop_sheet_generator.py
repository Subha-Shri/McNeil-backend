import sys
from reportlab.lib.pagesizes import letter, landscape
from reportlab.pdfgen import canvas
from reportlab.lib import colors

def generate_pdf(tag, filename="output.pdf"):
    c = canvas.Canvas(filename, pagesize=landscape(letter))
    width, height = landscape(letter) # 792 x 612 points

    # --- 1. Outer Border ---
    c.setLineWidth(1.5)
    c.setStrokeColor(colors.black)
    c.rect(36, 36, width - 72, height - 72)

    # --- 2. Title Block (Bottom Right) ---
    c.rect(width - 320, 36, 284, 60)
    c.line(width - 320, 66, width - 36, 66)
    
    c.setFont("Helvetica-Bold", 10)
    c.drawString(width - 310, 80, "PROJECT: McNeil Industrial Automation")
    
    # Clearly display the dynamic tag in the title block
    c.setFont("Helvetica-Bold", 10)
    c.setFillColor(colors.darkblue)
    c.drawString(width - 310, 50, f"LOOP DIAGRAM TAG: {tag}")
    
    c.setFillColor(colors.black)
    c.setFont("Helvetica", 9)
    c.drawString(width - 130, 50, "REV: 01")

    # --- 3. Process Flow & Instrument Diagram Area ---
    # Draw Instrument Bubble (ISA Standard Circle)
    c.setLineWidth(1.2)
    c.circle(180, height - 200, 45, stroke=1, fill=0)
    
    # Clearly display the dynamic tag inside the instrument bubble
    c.setFont("Helvetica-Bold", 12)
    c.drawCentredString(180, height - 195, tag)
    c.setFont("Helvetica", 8)
    c.drawCentredString(180, height - 212, "PLC / DCS")

    # Process Signal Lines
    c.setLineWidth(1.2)
    c.line(90, height - 200, 135, height - 200)
    c.drawString(90, height - 185, "4-20mA Process Input")

    c.line(225, height - 200, 340, height - 200)
    c.drawString(235, height - 185, "Control Signal Output")

    # Control Valve Symbol
    c.rect(340, height - 215, 30, 30, stroke=1, fill=0)
    c.line(340, height - 185, 370, height - 215)
    c.line(340, height - 215, 370, height - 185)
    c.setFont("Helvetica-Bold", 9)
    c.drawString(342, height - 235, "CV-101")

    # --- 4. Technical Parameters Table ---
    c.rect(36, 110, width - 72, 110)
    c.line(36, 190, width - 72, 190)
    
    c.setFont("Helvetica-Bold", 9)
    c.drawString(45, 200, "TERMINAL")
    c.drawString(140, 200, "WIRE / CABLE")
    c.drawString(280, 200, "DESCRIPTION / SIGNAL FUNCTION")
    c.drawString(560, 200, "PANEL / RACK")

    # Populate rows referencing the dynamic tag
    c.setFont("Helvetica", 9)
    rows = [
        ("TB1-01", "18 AWG Shielded (K-Type)", f"Primary Sensor Input for loop {tag}", "Rack 01, Slot 4"),
        ("TB1-02", "18 AWG Shielded (K-Type)", f"Signal Ground / Common Return ({tag})", "Rack 01, Slot 4"),
        ("TB2-11", "16 AWG Twisted Pair", f"Controller Analog Output (AO) for {tag}", "Rack 02, Slot 1")
    ]
    
    y_pos = 165
    for term, wire, desc, rack in rows:
        c.drawString(45, y_pos, term)
        c.drawString(140, y_pos, wire)
        c.drawString(280, y_pos, desc)
        c.drawString(560, y_pos, rack)
        y_pos -= 22

    c.save()

if __name__ == "__main__":
    tag_arg = sys.argv[1] if len(sys.argv) > 1 else "TAG-001"
    generate_pdf(tag_arg, "output.pdf")