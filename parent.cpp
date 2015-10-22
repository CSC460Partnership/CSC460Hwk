#include <iostream>
using namespace std;
#include <stdlib.h>
#include <unistd.h>

struct nums {
  float x, y, z;
};

int main()
{
  const int rHead = 0, wHead = 1;
  int       pipeToChild[2], anInt, bytes, pipeToParent[2];
  nums val;
  // For demo, skipped some failure detections for system calls !!!
  val.x = 5.1;
  val.y = 6.2;
  val.z = 7.3;
  //--------------------------------------
  // 1. Create a pipe
  //--------------------------------------
  if (pipe(pipeToChild) == -1)
  {
    cout << "\n*** Failed: child pipe(...) ***\n";
    exit(0);
  }
  if (pipe(pipeToParent) == -1)
  {
    cout << "\n*** Failed: parent pipe(...) ***\n";
    exit(0);
  }
  //--------------------------------------
  // 2. Create a child
  //--------------------------------------
  pid_t returnVal = fork();
  if (returnVal == 0)
  {
    /*
    execl("child", "", &pipeToChild[rHead], &pipeToParent[wHead], NULL);
    perror("execl");
    close(pipeToChild[rHead]);
    close(pipeToChild[wHead]);
    close(pipeToParent[rHead]);
    close(pipeToParent[wHead]);*/

    nums temp;
    close(pipeToChild[wHead]);	     // the child only reads from this pipe

    bytes = read(pipeToChild[rHead],  &temp, sizeof(temp));
    if (bytes != sizeof(val))
    {
      cout << "\n*** Error from child: read(...) ***\n";
      close(pipeToChild[rHead]);
      exit(0);
    }

    cout <<"\n============================================================\n";
    cout <<"Child(" <<getpid() <<") gets an object from the parent: " << temp.x;
    cout <<"\n============================================================\n\n";

    close(pipeToChild[rHead]);	// done with reading
    ////////////////// start of new pipe stuff ///////////////////////////////

    temp.x = 8.9; // change part of the object to see of changes go through the
                  // pipe
    close(pipeToParent[rHead]);	// child only writes to this pipe
    //--------------------------------------
    // 4. Write to the parent
    //--------------------------------------
    bytes = write(pipeToParent[wHead], &temp, sizeof(temp));
    if (bytes != sizeof(temp))
      cout << "\n*** Error from child: write(...) ***\n";

    close(pipeToParent[wHead]);	// done with writing

    exit(0);
    
  }
  else if ( returnVal > 0)
  {
    // only the parent reaches here
    close(pipeToChild[rHead]);	// parent only writes to this pipe
    //--------------------------------------
    // 4. Write to the child
    //--------------------------------------
    bytes = write(pipeToChild[wHead], &val, sizeof(val));
    if (bytes != sizeof(val))
      cout << "\n*** Error from parent 1: write(...) ***\n";

    close(pipeToChild[wHead]);	// done with writing

    /////////////// start of new pipe stuff /////////////////////////
    close(pipeToParent[wHead]);

    bytes = read(pipeToParent[rHead],  &val, sizeof(val));
    if (bytes != sizeof(val))
    {
      cout << "\n*** Error from child in parent 1: read(...) ***\n";
      close(pipeToParent[rHead]);
      exit(0);
    }

    cout <<"\n============================================================\n";
    cout <<"Parent(" <<getpid() <<") gets an object from the child: " << val.x;
    cout <<"\n============================================================\n\n";

    close(pipeToChild[rHead]);	// done with reading
  }

  //--------------------------------------
  // 5. Wrap up
  //--------------------------------------
  return 0;
}
