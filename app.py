from fastapi import FastAPI
from fastapi.responses import FileResponse, HTMLResponse
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
import tempfile
import os

app = FastAPI(title="McNeil Loop Sheet Engine")

@app.get("/", response_class=HTMLResponse)
def read_root():
    return """
    <html>
        <head>
            <title>McNeil Loop Sheet Portal</title>
            <style>
                body { font-family: Arial, sans-serif; background-color: #f4f6f9; text-align: center; padding-top: 50px; }
                .card { background: white; padding: 40px; border-radius: 10px; box-shadow: 0px 4px 10px rgba(0,0,0,0.1); display: inline-block; }
                input { padding: 10px; font-size: 16px; width: 250px; margin-right: 10px; border: 1px solid #ccc; border-radius: 5px; }
                button { padding: 10px 20px; font-size: 16px; background-color: #007acc; color: white; border: none; border-radius: 5px; cursor: pointer; }
                button:hover { background-color: #005fa3; }
            </style>
        </head>
        <body>
            <div class="card">
                <h2>McNeil Loop Sheet Generator</h2>
                <p>Status: <span style="color: green; font-weight: bold;">Engine is running!</span></p>
                <br>
                <input type="text" id="tagInput" placeholder="Enter Tag ID (e.g., TAG-101)" value="TAG-101">
                <br><br>
                <button onclick="generatePDF()">Download Loop Sheet PDF</button>
            </div>
            <script>
                function generatePDF() {
                    const tag = document.getElementById('tagInput').value;
                    if(!tag) { alert('Please enter a Tag ID'); return; }
                    window.location.href = '/generate-loop-sheet/' + tag;
                }
            </script>
        </body>
    </html>
    """

@app.get("/generate-loop-sheet/{tag_id}")
def generate_loop_sheet(tag_id: str):
    temp_dir = tempfile.gettempdir()
    file_path = os.path.join(temp_dir, f"Loop_Sheet_{tag_id}.pdf")
    
    c = canvas.Canvas(file_path, pagesize=letter)
    c.drawString(100, 750, f"McNeil Project - Loop Sheet: {tag_id}")
    c.drawString(100, 730, "System Status: Validated via Render Cloud")
    c.save()
    
    return FileResponse(file_path, media_type='application/pdf', filename=f"Loop_Sheet_{tag_id}.pdf")