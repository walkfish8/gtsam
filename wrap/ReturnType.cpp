/**
 * @file ReturnType.cpp
 * @date Nov 13, 2014
 * @author Frank Dellaert
 */

#include "ReturnType.h"
#include "Class.h"
#include "utilities.h"
#include <iostream>

using namespace std;
using namespace wrap;

/* ************************************************************************* */
string ReturnType::str(bool add_ptr) const {
  return maybe_shared_ptr(add_ptr && isPtr, qualifiedName("::"), name());
}

/* ************************************************************************* */
void ReturnType::wrap_result(const string& out, const string& result,
    FileWriter& wrapperFile, const TypeAttributesTable& typeAttributes) const {

  string cppType = qualifiedName("::"), matlabType = qualifiedName(".");

  if (category == CLASS) {

    // Handle Classes
    string objCopy, ptrType;
    const bool isVirtual = typeAttributes.attributes(cppType).isVirtual;
    if (isPtr)
      objCopy = result; // a shared pointer can always be passed as is
    else {
      // but if we want an actual new object, things get more complex
      if (isVirtual)
        // A virtual class needs to be cloned, so the whole hierarchy is returned
        objCopy = result + ".clone()";
      else
        // ...but a non-virtual class can just be copied
        objCopy = "Shared" + name() + "(new " + cppType + "(" + result + "))";
    }
    // e.g. out[1] = wrap_shared_ptr(pairResult.second,"gtsam.Point3", false);
    wrapperFile.oss << out << " = wrap_shared_ptr(" << objCopy << ",\""
        << matlabType << "\", " << (isVirtual ? "true" : "false") << ");\n";

  } else if (isPtr) {

    // Handle shared pointer case for BASIS/EIGEN/VOID
    wrapperFile.oss << "  {\n  Shared" << name() << "* ret = new Shared" << name()
        << "(" << result << ");" << endl;
    wrapperFile.oss << out << " = wrap_shared_ptr(ret,\"" << matlabType
        << "\");\n  }\n";

  } else if (matlabType != "void")

    // Handle normal case case for BASIS/EIGEN
    wrapperFile.oss << out << " = wrap< " << str(false) << " >(" << result
        << ");\n";

}

/* ************************************************************************* */
void ReturnType::wrapTypeUnwrap(FileWriter& wrapperFile) const {
  if (category == CLASS)
    wrapperFile.oss << "  typedef boost::shared_ptr<" << qualifiedName("::")
        << "> Shared" << name() << ";" << endl;
}

/* ************************************************************************* */
void ReturnType::emit_cython_pxd(FileWriter& file, const std::string& className) const {
  string typeName = cythonClass();
  string cythonType;
  if (isEigen()) cythonType = typeName + "Xd";
  else if (isPtr) cythonType = "shared_ptr[" + typeName + "]";
  else cythonType = typeName;
  if (typeName == "This") cythonType = className;
  file.oss << cythonType;
}

/* ************************************************************************* */
void ReturnType::emit_cython_pyx_return_type(FileWriter& file) const {
  string retType = pyxCythonClass();
  if (isPtr) retType = "shared_ptr[" + retType + "]";
  file.oss << retType;
}

/* ************************************************************************* */
void ReturnType::emit_cython_pyx_casting(FileWriter& file, const std::string& var) const {
  if (isEigen())
    file.oss << "ndarray_copy";
  else if (isNonBasicType()){
    if (isPtr) 
      file.oss << pythonClass() << ".cyCreateFromShared";
    else {
      // if the function return an object, it must be copy constructible and copy assignable
      // so it's safe to use cyCreateFromValue
      file.oss << pythonClass() << ".cyCreateFromValue";
    }
  }
  file.oss << "(" << var << ")";
}

/* ************************************************************************* */

