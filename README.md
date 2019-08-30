# __docgen__

*Work in progress :)

Docgen is a tool that generates python project documentation based on the comments written in source code files.

# Language Overview
Documentation is generated from specific instructions placed in source code files as comments.

All docgen instructions begin with a "dg" keyword.

    #dg <--- this marks the beginning of a docgen instruction.

    //dg <--- this works for js, c++, java, etc. As of right now, however, docgen is designed for python.

Docgen contains 4 basic instructions:

* START
* END
* RETURNS
* BRIEF

In addition, there are 4 "types" that can be STARTed and ENDed.

* CLASS
* FUNC
* ARGS
* DESC