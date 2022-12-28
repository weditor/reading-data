# reading-data

## python test

```python
import example
import json

path = "../data/usk052/content.json"
fp = open(path, 'r', encoding="utf-8")
a = fp.readline()
b = json.loads(a)
c = b['contents'][0]['content']
d = example.SimpleHtmlDoc.parse_raw(c)
items = list(d.items)
aa = items[-1]
```