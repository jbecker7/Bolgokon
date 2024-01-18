# Bolgokon (ᠪᠣᠯᡤᠣᡴᠣᠨ)
In Manchu, ᠪᠣᠯᡤᠣᡴᠣᠨ (bolgokon) means "somewhat clean," and this is an application that makes your files and overall workflow just that. With a bunch of neat features built to fulfill my extremely niche needs, there's something for everyone! No guarantees about exactly how clean things will be after you start using it but... somewhat clean is probably a good guess.

Why did I choose a Manchu name, you ask? Well, I think there's a ton of random pieces of software with Japanese names so.. why not use Manchu instead to drum up interest in a critically endangered language?

I am going to add at least 3 features per menu optio  plus a settings panel and write actual documentation once it's all done.

## Usage
1. Clone the repo
2. Compile each of the C++ files
3. Run `main_gui.py`


## Text Menu

### 1. Hanzi Harvester

Hanzi Harvester allows the user to choose a .txt document in Chinese and quickly extracts all of the unique characters using cutting edge data structures (lol I used a set) to a file called `characters.txt`

## Photos Menu

### 1. File Sorter

File Sorter is a boiled down version of File Hoarder Fiesta, the "acclaimed" C++ app I made last week to clean up my screenshots. Simply pick the directory you want to clean up, and it will quickly move all of the files that contain "Screenshot" into a folder called Images in the same directory.

## Videos Menu 

### 1. Video Markdown

Video Markdown is also an improved version of a C++ app I have made in the last week lol. Choose the directory where you store your video files and it will quickly generate a convenient markdown to-do list called `video_queue.md`

## Misc Menu

### 1. (Show/Hide) Card Search

Card Search is pretty cool, though I admit it does not really fit the theme of this application per se haha. I have been playing Yu-Gi-Oh Duel Links recently, and it can be a pain to go and Google the effects of cards and whatnot, so I added this nifty feature to search cards. I couldn't have done it without the [YGOPRODeck API](https://ygoprodeck.com/api-guide/)or this great [C++ JSON library](https://json.nlohmann.me/). I am kind of proud of the suggestions feature--you can type in a term like "Dark" and it will give you all the cards containing "Dark" so you can figure out which specific card you wanted to search for and type its name in.












(It's kind of like I took all of the weirdly specific things I might write spaghetti code C++ scripts to do and fused them all together to form... a big pile of spaghetti!)
