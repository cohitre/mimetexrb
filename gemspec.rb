require 'rubygems'

spec = Gem::Specification.new do | s |
  s.name       = "mimetexrb"
  s.version    = "1.0.0"
  s.author     = "32leaves"
  s.email      = "info@32leaves.net"
  s.homepage   = "http://blog.32leaves.net"
  s.platform   = Gem::Platform::RUBY
  s.summary    = "Native bindings for the MimeTex library"
  s.files      = Dir['**/*.*']
  s.extensions = [ 'ext/extconf.rb' ]
end

if $0 == __FILE__
  Gem::manage_gems
  Gem::Builder.new( spec ).build
end
