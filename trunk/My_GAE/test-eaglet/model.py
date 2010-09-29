from google.appengine.ext import db
class Greeting(db.Model):
  author = db.UserProperty()
  content = db.StringProperty(multiline=True)
  date = db.DateTimeProperty(auto_now_add=True)

class Menu(db.Model):
    text = db.StringProperty(multiline=False)
    link = db.StringProperty(multiline=False)
    
