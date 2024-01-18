import tkinter as tk
from tkinter import filedialog
import threading
import subprocess
import os
import requests
from PIL import Image, ImageTk
import io

# Paths to executables

current_directory = os.path.dirname(os.path.abspath(__file__))
VM_PATH = os.path.join(current_directory, "video_markdown")
FS_PATH = os.path.join(current_directory, "file_sorter")
HH_PATH = os.path.join(current_directory, "hanzi_harvester")
CS_PATH = os.path.join(current_directory, "card_search")


# Variables for GUI
is_card_search_visible = False


# These functions are for appearance stuff


def append_output(text):
    output_text.configure(state="normal")
    output_text.insert(tk.END, text + "\n")
    output_text.see(tk.END)
    output_text.configure(state="disabled")

    # Check each line of the output for an image URL
    for line in text.split("\n"):
        if line.startswith("Image URL:"):
            # Extract and display the image
            url = line.split("Image URL:")[1].strip()  # Extract the URL
            display_image(url)
            break


def setup_card_search():
    global is_card_search_visible
    if is_card_search_visible:
        # Hide card search widgets and update menu label
        for widget in search_frame.winfo_children():
            widget.destroy()
        misc_menu.entryconfig("Hide Card Search", label="Show Card Search")
        is_card_search_visible = False
    else:
        # Show card search widgets and update menu label
        for widget in search_frame.winfo_children():
            widget.destroy()

        search_entry = tk.Entry(search_frame, width=30)
        search_entry.pack(side=tk.LEFT, padx=5)

        search_button = tk.Button(
            search_frame, text="Card Search", command=lambda: cs(search_entry.get())
        )
        search_button.pack(side=tk.LEFT, padx=5)

        misc_menu.entryconfig("Show Card Search", label="Hide Card Search")
        is_card_search_visible = True


def display_image(image_url):
    try:
        response = requests.get(image_url)
        image_data = response.content
        image = Image.open(io.BytesIO(image_data))
        image.thumbnail((200, 300))  # Resize the image if necessary
        photo = ImageTk.PhotoImage(image)

        image_label.configure(image=photo)
        image_label.image = photo  # Keep a reference
    except Exception as e:
        print(f"Error fetching image: {e}")


# These functions are for running the executables


def run_in_thread(command, path=None):
    def target():
        if path:
            subprocess.run(command, cwd=path)
        else:
            subprocess.run(command)
        append_output(f"Processed {command[0]} on {path if path else 'file selected'}")

    thread = threading.Thread(target=target)
    thread.start()


def vm():
    directory = filedialog.askdirectory()
    if directory:
        run_in_thread([VM_PATH], directory)


def fs():
    directory = filedialog.askdirectory()
    if directory:
        run_in_thread([FS_PATH], directory)


def hh():
    file = filedialog.askopenfilename()
    if file:
        run_in_thread([HH_PATH, file])


# I am leaving these debugging statements in for now because this one can be volatile
def run_cpp_search_in_thread(search_term):
    def target():
        command = [CS_PATH, search_term]
        try:
            result = subprocess.run(command, capture_output=True, text=True, check=True)
            output = result.stdout
            print(f"Output received: {output}")  # Debugging statement
        except subprocess.CalledProcessError as e:
            output = f"Error: {e.output}"
            print(f"Subprocess error: {output}")  # Debugging statement
        except Exception as e:
            output = f"An error occurred: {e}"
            print(f"Exception: {output}")  # Debugging statement
        app.after(0, append_output, output)

    thread = threading.Thread(target=target)
    thread.start()


def cs(search_term):
    run_cpp_search_in_thread(search_term)


# The main GUI
app = tk.Tk()
app.title("ᠪᠣᠯᡤᠣᡴᠣᠨ Bolgokon")
app.geometry("800x600")

# Create a frame for the top bar
top_bar = tk.Frame(app, bg="gray", bd=2)
top_bar.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)

# Create a frame for the buttons
buttons_frame = tk.Frame(top_bar)  # Parent is 'top_bar'
buttons_frame.pack(side=tk.TOP, pady=10)  # Centered in 'top_bar'

# Initialize an empty search_frame for dynamic addition of search widgets
search_frame = tk.Frame(top_bar)
search_frame.pack(side=tk.TOP, pady=10)

# Create menubuttons inside the buttons_frame
text_button = tk.Menubutton(buttons_frame, text="Text", relief=tk.RAISED)
photo_button = tk.Menubutton(buttons_frame, text="Photos", relief=tk.RAISED)
video_button = tk.Menubutton(buttons_frame, text="Videos", relief=tk.RAISED)
misc_button = tk.Menubutton(buttons_frame, text="Misc", relief=tk.RAISED)

# Create menus
text_menu = tk.Menu(text_button, tearoff=0)
photo_menu = tk.Menu(photo_button, tearoff=0)
video_menu = tk.Menu(video_button, tearoff=0)
misc_menu = tk.Menu(misc_button, tearoff=0)

# Add commands to menus
photo_menu.add_command(label="File Sorter", command=fs)
video_menu.add_command(label="Video Markdown", command=vm)
text_menu.add_command(label="Hanzi Harvester", command=hh)
misc_menu.add_command(label="Show Card Search", command=setup_card_search)

# Associate menus with menubuttons
text_button["menu"] = text_menu
photo_button["menu"] = photo_menu
video_button["menu"] = video_menu
misc_button["menu"] = misc_menu

# Pack menubuttons inside the buttons_frame
text_button.pack(side=tk.LEFT, padx=5)
photo_button.pack(side=tk.LEFT, padx=5)
video_button.pack(side=tk.LEFT, padx=5)
misc_button.pack(side=tk.LEFT, padx=5)

# Console-like output text area
output_frame = tk.Frame(app, bg="black")
output_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
image_label = tk.Label(output_frame)
image_label.pack(pady=10)

output_text = tk.Text(output_frame, bg="black", fg="white", state="disabled")
output_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

# Add search functionality in the GUI
search_frame = tk.Frame(top_bar)
search_frame.pack(side=tk.TOP, pady=10)


app.mainloop()
