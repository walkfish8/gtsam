// automatically generated by wrap on 2011-Dec-08
#include <wrap/matlab.h>
#include <ClassC.h>
void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[])
{
  checkArguments("new_ns2ClassC_",nargout,nargin,0);
  ns2::ClassC* self = new ns2::ClassC();
  out[0] = wrap_constructed(self,"ns2ClassC");
}
