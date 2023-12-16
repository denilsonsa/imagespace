# ImageSpace

ImageSpace is a genetic image generator, that means it generates and mutates
mathematical functions while the user's aesthetic perception is the fitness
function, selecting an image which will be mutated. Since the functions are
represented by syntax trees, the search space is really infinite (though your
computer's capacity will limit your voyages).

ImageSpace allows you to choose some parameters like syntax tree depth/width,
mutation probability, operators etc.

It is open-source under the GPL license.

## Requisites

* C++ compiler (GCC)
* Qt5

It builds and runs fine on Linux. Earlier versions were known to also work on
Mac OS X and Windows, so please try it yourself on those systems.

## Compiling

Use this simple script to generate a `Makefile`:

    ./configure

Then just compile it using `make`, optionally passing `-j` to speed up the
compilation if you have multiple CPUs:

    make -j 4

## Running

Just run it:

    ./imagespace

Feel free to install this binary on your system. It is self-contained, it has
no dependencies on any other file (other than the shared libraries).

## Troubleshooting

Qt libraries need to be built with multithreading support. But this is probably
always the case nowadays.

## Credits

Original code written by Ewald Krämer (from Subsymbolics GmbH, Germany) between
2003 and 2006.

In 2023, Denilson Sá Maia imported the code into git and fixed it to make it
compile and run again. However, he has no intention to keep developing this
code. If anyone else wants to adopt this project, go ahead!

## Future work

Do you want to contribute to this project? Here's some work that needs to be done:

* [ ] Resolve the deprecated use of [QLabel::pixmap()](https://doc.qt.io/qt-5/qlabel-obsolete.html#pixmap).
* [ ] Resolve the compiler warnings (about unused parameters).
* [ ] Port from Qt5 to Qt6.
* [ ] Create GitHub actions to automatically build binaries for Linux, Windows and Mac. [install-qt-action](https://github.com/marketplace/actions/install-qt) can be helpful.

Please also consider adopting this project.

## Related links

* <https://imagespace.sourceforge.net/> - Original homepage (old, not updated since 2006).
* [Evolvotron](https://www.timday.com/share/evolvotron/index.html) ([GitHub project](https://github.com/WickedSmoke/evolvotron)) is another generative art application, also written using Qt.
* [Kandid](https://kandid.sourceforge.net/) ([SourceForge project](https://sourceforge.net/projects/kandid/)) is a genetic art project written in Java. There is a [large selection of links for further exploration](https://kandid.sourceforge.net/links.html).
* Wikipedia's articles on [Evolutionary art](https://en.wikipedia.org/wiki/Evolutionary_art) and on [Generative art](https://en.wikipedia.org/wiki/Generative_art).
