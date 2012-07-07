require 'mkmf'

dir_config("mimetexrb")

with_cflags("-DAA") do
  create_makefile("mimetexrb", "src")
end
