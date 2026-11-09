from fastapi import FastAPI
from fastapi.responses import FileResponse
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
import tempfile
import os

app = FastAPI(title="McNeil Loop Sheet Engine")

@app.get("/")
def read_root():
    return {"status": "Python Loop Sheets Engine is running!","status": "Progressing - GS"}
@app.get("/generate-loop-sheet/{tag_id}")
def generate_loop_sheet(tag_id: str):
    temp_dir = tempfile.gettempdir()
    file_path = os.path.join(temp_dir, f"Loop_Sheet_{tag_id}.pdf")
    
    c = canvas.Canvas(file_path, pagesize=letter)
    c.drawString(100, 750, f"McNeil Project - Loop Sheet: {tag_id}")
    c.drawString(100, 730, "System Status: Validated")
    c.save()
    
    return FileResponse(file_path, media_type='application/pdf', filename=f"Loop_Sheet_{tag_id}.pdf")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000)