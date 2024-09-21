import random
import string

f = open("file.txt", "a")

while True:
    # Generate a random string of 10 characters
    random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=10))
    
    # Write the random string to the file
    f.writelines(random_string + "\n")
