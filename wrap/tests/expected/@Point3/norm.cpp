// automatically generated by wrap on 2011-Dec-09
#include <wrap/matlab.h>
#include <Point3.h>
using namespace geometry;
void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[])
{
  checkArguments("norm",nargout,nargin-1,0);
  shared_ptr<Point3> self = unwrap_shared_ptr< Point3 >(in[0],"Point3");
  double result = self->norm();
  out[0] = wrap< double >(result);
}
