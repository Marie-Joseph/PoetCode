import random
from datetime import datetime


with open("dicts/large") as fp:
    words = list(fp)

with open("poems/Py-" + datetime.now().isoformat(), "w") as op:
    for stanza in range(3):
        for line in range(12):
            for word in range(random.randrange(13)):
                op.write(words[random.randrange(len(words))].rstrip() + " ")
            op.write("\n")
        op.write("\n")
