import os
from google.appengine.ext.webapp import template
import cgi

from google.appengine.api import users
from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from model import *

class MainPage(webapp.RequestHandler):
  def get(self):
    greetings_query = Greeting.all().order('-date')
    greetings = greetings_query.fetch(10)

    if users.get_current_user():
      url = users.create_logout_url(self.request.uri)
      url_linktext = 'Logout'
    else:
      url = users.create_login_url(self.request.uri)
      url_linktext = 'Login'

    template_values = {
      'greetings': greetings,
      'url': url,
      'url_linktext': url_linktext,
      }

    path = os.path.join(os.path.dirname(__file__), 'index.html')
    self.response.out.write(template.render(path, template_values))
    
class PutGuestBook(webapp.RequestHandler):
  def post(self):
    greeting = Greeting()

    if users.get_current_user():
      greeting.author = users.get_current_user()

    greeting.content = self.request.get('content')
    greeting.put()
    self.redirect('/GetGuest')
    
class GuestBook(webapp.RequestHandler):
  def get(self):
    template_values = {}
    path = os.path.join(os.path.dirname(__file__), 'Guestbook.html')
    self.response.out.write(template.render(path, template_values))
    
class GetGuest(webapp.RequestHandler):
  def get(self):
    greetings_query = Greeting.all().order('-date')
    greetings = greetings_query.fetch(10)
    rp = "" 
    for greeting in greetings:
      if greeting.author:
        rp += "<div class='guestname'>" + greeting.author.nickname + "</div>"
      else:
        rp += "<div class='guestname'>An anonymous person</div>"
      content = greeting.content.replace("<","&lt;").replace(">","&gt;")
      rp += "<div class='guestcontent'>" + content +"</div>"
    self.response.out.write(rp)
    
class MyInformation(webapp.RequestHandler):
  def get(self):
    template_values = {}
    path = os.path.join(os.path.dirname(__file__), 'MyInformation.html')
    self.response.out.write(template.render(path, template_values))

application = webapp.WSGIApplication(
                                     [('/', MainPage),
                                      ('/GuestBook', GuestBook),
                                      ('/GetGuest', GetGuest),
                                      ('/PutGuestBook', PutGuestBook),
                                      ('/MyInformation', MyInformation)
                                      ],
                                     debug=True)

def main():
  run_wsgi_app(application)

if __name__ == "__main__":
  main()
