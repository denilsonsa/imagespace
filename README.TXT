You will need a QT 4.x developer library built for multithreading support.
To do this, download QT4 from Trolltech and configure it with
./configure

Then start make and go for lots of coffees
After QT is built, don't use make install, instead, export the directory
of QT to $QTDIR
ImageSpace has no real config script, instead it uses the qmake system.
Therefore, a QT4 qmake should be found somewhere inside your search tree.
Also, $QMAKESPEC has to point to a file in the QT tree defining your
system and compiler. On linux with the bash shell, this should be something
like that:

export QTDIR=/home/QT401
export QMAKESPEC=$QTDIR/mkspecs/linux-g++
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

Now you should be able to configure ImageSpace by changing to it's directory
and starting ./configure

If everything works, a makefile should be generated now.
You can also create it without ./configure by typing
qmake -o Makefile imagespace.pro
If qmake is unknown on your system, it can be found and build manually
in $QTDIR/tools/qmake

POSSIBLE PROBLEMS: 
Most older Linux versions come without QT4. If you don't build your own QT4,
the qmake version will most likely be a QT3 qmake, which will not work.


Now start make, it will need only some seconds and imagespace is built.

Copy it to a location in your path, or start it with
./imagespace

And remember: If something goes wrong, try to check your QT system first.
It NEEDS to be built with multithreading support, and it needs to be a 4.x
version.

For any questions, mail me:

kraemer@subsymbolics.com
