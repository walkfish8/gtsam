// automatically generated by wrap on 2011-Dec-09
#include <wrap/matlab.h>
#include <folder/path/to/Test.h>
using namespace geometry;
void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[])
{
  checkArguments("return_size_t",nargout,nargin-1,1);
  shared_ptr<Test> self = unwrap_shared_ptr< Test >(in[0],"Test");
  size_t value = unwrap< size_t >(in[1]);
  size_t result = self->return_size_t(value);
  out[0] = wrap< size_t >(result);
}
