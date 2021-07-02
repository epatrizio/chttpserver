# chttpserver - Minimalist and naive http server in C language

I have 20 years of experience in web programming (mainly PHP and Javascript languages).
Actually (2021), I'm in a learning period (exciting). Good!

How does a web server work over http protocol? Very simple question ;) But, i don't know the answer, and it's a basic server component, OMG!

So, this side project simply helps me answer this fundamental question. It's therefore a small web server, written in C language, which experiments sockets programming (mechanism proposed by computer OS allowing programs to network access), and back to bacics regards http/1 protocol.
My implementation is inspired by this [blog post](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa), this other one [blog post](https://dev-notes.eu/2018/06/http-server-in-c/) and this [project](http://tinyhttpd.sourceforge.net/).

# Hypertext Transfer Protocol

A web server implements HTTP protocol (an application layer protocol on TCP Transmission Control Protocol, UDP User Datagram Protocol and IP Internet Protocol, the network layer).
* [HTTP/1](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)
* [HTTP/2](https://en.wikipedia.org/wiki/HTTP/2) (New version, V1 compatible)

Here, I only implement the basics of http/1 (main headers and methods). It's obviously not a question of implementing whole HTTP (A big and complex stuff ;).

So, for now, chttpserver support static contents (html, css, javascript, images files).
And PHP CGI (see below) for handling form post and simple data processing.

With this core features, it becomes possible to run a nice little website. Cool!

# Common Gateway Interface

To have a nice website, static contents are not sufficient. Quickly, you would like to manipulate data, and generate pages dynamically. Then, you have to use a programming language, you have to interact with a program.
[CGI - Common Gateway Interface](https://en.wikipedia.org/wiki/Common_Gateway_Interface) help us.
Wikipedia : "CGI is an interface specification that enables web servers to execute an external program.".

So, in chttpserver, i experiment php-cgi. This is the original approach for PHP, but which is now outdated, and replaced by [php-fpm](https://www.php.net/manual/en/install.fpm.php) (FastCGI Process Manager). In our context, that's not a problem.

# Run chttpserver

Execute "make compile" and that's all!
Note that server root is fixed at /home/wwwroot directory.
# Run tests

Execute "make compile_test" and that's all!
I use the framework [ctestfmk](https://github.com/epatrizio/ctestfmk). Read the documentation for more informations.
