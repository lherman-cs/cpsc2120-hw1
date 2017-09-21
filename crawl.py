

with open('pages.txt') as i:
   pages = [line.strip() for line in i]

print(type(pages), len(pages))

with open('links.txt') as links:
   links = [link.strip() for link in links]
   
for link in links:
   if not link in pages:
      print(link)
