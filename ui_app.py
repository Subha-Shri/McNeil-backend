import tkinter as tk
from tkinter import filedialog, messagebox
import urllib.request
import webbrowser

def browse_file():
    file_path = filedialog.askopenfilename(
        title="Select Configuration or Data File",
        filetypes=[("All Files", "*.*"), ("PDF Files", "*.pdf"), ("CSV Files", "*.csv"), ("Text Files", "*.txt")]
    )
    if file_path:
        lbl_path.config(text=f"Selected File: {file_path}")
        lbl_status.config(text="Status: File loaded locally.")

def call_render_cloud():
    try:
        lbl_status.config(text="Status: Connecting to Render cloud engine...")
        root.update_idletasks()
        
        # URL of your live Render backend endpoint for a sample tag
        url = "https://mcneil-backend.onrender.com/generate-loop-sheet/TAG-101"
        
        # Open and download the PDF from Render
        output_filename = "Downloaded_Loop_Sheet_TAG-101.pdf"
        urllib.request.urlretrieve(url, output_filename)
        
        lbl_status.config(text="Status: Success! PDF downloaded locally.")
        messagebox.showinfo("Success", f"Loop Sheet PDF successfully downloaded from Render and saved as:\n{output_filename}")
    except Exception as e:
        lbl_status.config(text="Status: Connection failed.")
        messagebox.showerror("Error", f"Failed to connect to Render cloud:\n{e}")

# Create the main window
root = tk.Tk()
root.title("McNeil Project - Local UI & Cloud Engine")
root.geometry("550x260")
root.resizable(False, False)

# Styling UI Elements
btn_browse = tk.Button(root, text="Browse File", font=("Arial", 11, "bold"), bg="#e0e0e0", command=browse_file, width=18, height=2)
btn_browse.place(x=50, y=30)

btn_cloud = tk.Button(root, text="Generate Cloud Loop Sheet", font=("Arial", 11, "bold"), bg="#007acc", fg="white", command=call_render_cloud, width=22, height=2)
btn_cloud.place(x=260, y=30)

lbl_path = tk.Label(root, text="Selected File: None", font=("Arial", 10), fg="#333333", anchor="w")
lbl_path.place(x=50, y=120, width=450, height=25)

lbl_status = tk.Label(root, text="Status: Ready", font=("Arial", 10, "italic"), fg="#006600", anchor="w")
lbl_status.place(x=50, y=160, width=450, height=25)

root.mainloop()