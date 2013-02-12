require 'rake/clean'

OBJECTS = FileList["src/**/*.c"].ext(".o")
CLEAN.include OBJECTS
CLEAN << "build"

CFLAGS = "-g0 -O3 -Os -Iinclude/"
CC = "gcc"
RE2CFLAGS = "-g"

rule ".o" => ".c" do |t|
    sh "re2c #{RE2CFLAGS} #{t.source} | #{CC} #{CFLAGS} -c -o #{t.name} -xc -"
end

task :objects => OBJECTS

directory "build"

file "build/froscio" => [:objects, "build"] do |t|
    sh "gcc -o #{t.name} #{OBJECTS.join " "}"
end


task :default => [:clean, "build/froscio"]

