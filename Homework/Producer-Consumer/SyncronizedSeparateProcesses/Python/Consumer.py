import os

while True:
    with open("file.txt", "r+", encoding="utf-8") as file:
        # Check if the file is empty
        if os.stat("file.txt").st_size == 0:
            print("File is empty.")
        else:
            # Move the pointer to the end of the file
            file.seek(0, os.SEEK_END)

            # Get the current position of the pointer (last character)
            pos = file.tell() - 1

            # Move backwards until a newline is found
            while pos > 0:
                file.seek(pos)
                char = file.read(1)
                if char == "\n":
                    break
                pos -= 1

            # Read and print the last line before truncating
            file.seek(pos + 1)  # Move to the start of the last line
            last_line = file.read().strip()
            print(f"Deleted line: {last_line}")

            # Truncate the file to remove the last line
            file.seek(pos)
            file.truncate()
