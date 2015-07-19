[Blog Post from July 19, 2015](http://www.brettmorrison.com/a-look-back-to-1991-code-programming-windows-3-in-c)

The Complete Golf Handicapper for Windows was developed as an independent study project at the [University of Arizona](http://www.cs.arizona.edu/) in 1991.  It was my last semester in Computer Science, and I needed 1 more unit to complete my degree.

At the time in an academic setting, Windows was a very unknown, mysterious, and dismissed Operating System.  I knew that Microsoft was on to something with Windows, and I knew it would be popular, so I taught myself how to develop for Windows with [Charles Petzold](http://www.charlespetzold.com/pw5/ProgWinEditions.html)'s seminal book [Programming Windows](http://www.worldcat.org/title/programming-windows-the-microsoft-guide-to-writing-applications-for-windows-3/oclc/263173436) as my bible.

I hard-pitched my Computer Science Professor, [Robert Drabek](http://www.brettmorrison.com/an-open-letter-to-my-computer-science-professor-robert-drabek) the idea of a native Windows GUI program where you could enter in your actual per-hole golf scores, compare them to the course certified par, and it would compute your handicap for that round, and keep history for all your rounds.  It took some convincing - he wanted me to do it in Unix/[X11](https://en.wikipedia.org/wiki/X_Window_System), but I eventually talked him into letting me write it for Windows 3.0.

The only language back then to write Windows programs in was C.  I used Microsoft's [C 6.0](https://en.wikipedia.org/wiki/Visual_C++#cite_note-4) Compiler, which was like $600, so I "borrowed" a copy from a friend and coded away.
![Original 3.5" Floppy Containing Source Code](http://www.brettmorrison.com/wp-content/uploads/2015/07/Golf-Handicapper.jpg)
I found this 3.5" floppy disk in my garage the other day and thought it would be fun to see it, so I ordered a [USB Floppy Drive from Amazon](http://amzn.com/B00BLTW48U), and much to my pleasant surprise - 24 years later, the disk was still readable, and the code all there.

I post to Github for historical significance under the "[public domain](https://en.wikipedia.org/wiki/Public_domain)" spirit, being how old it is.  :-)

It's interesting to look at how low-level I had to write to get the simplest things done.  Programming Languages and Application Frameworks have come a long way since 1991!

Brett
