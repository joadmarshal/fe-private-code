import cgi
from google.appengine.api import users
from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app

class MainPage(webapp.RequestHandler):   
	def get(self):     
		self.response.out.write("""       
		<html>         
		<body>           
		<form action="/sign" method="post">
		             <div><textarea name="content" rows="3" cols="60"></textarea></div>
		             <div><input type="submit" value="Sign Guestbook"></div>
		</form>
		</body>
		</html>""")   
		
class Guestbook(webapp.RequestHandler):
  def post(self):
    user = users.get_current_user()

    if user:
      self.response.headers['Content-Type'] = 'text/plain'
      self.response.out.write('Hello, ' + user.nickname())
    else:
      self.redirect(users.create_login_url(self.request.uri))
      
    self.response.out.write('aaaa')

application = webapp.WSGIApplication(
                                     [('/', MainPage),('/sign',Guestbook)],
                                     debug=True)

def main():
  run_wsgi_app(application)

if __name__ == "__main__":
  main()