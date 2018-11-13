#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define EMPTY       0       /* A space with no mines */
#define VALUE       15      /* Mask to turn on only the bits that say how many adjacent mines there are */
#define MINE        16      /* A space that contains a mine */
#define UNCOVERED   32      /* Flag that indicates a space has been uncovered */
#define FLAGGED     64      /* Flag that indicates a space has been flagged */
#define CURSOR      1       /* Cursor coloring */
#define FLAG        2       /* Flag coloring */

#endif
