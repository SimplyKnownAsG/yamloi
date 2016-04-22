    /* This file is input to SWIG, to inform SWIG about what it will be making...
 */

%module(directors="1") yamloi
%include "exception.i"
%include "stdint.i"
%include "std_string.i"
%include "std_shared_ptr.i"
%include "typemaps.i"
%include "std_vector.i"
%include "std_complex.i"


%{
/* Include statements for compiling SWIG source
 */
#include "yamloi.hpp"

%}

%exception {
    try {
        $action
    } catch(yamloi::TypeError &ex) {
        SWIG_exception(SWIG_TypeError, ex.what().c_str());
    } catch(yamloi::Exception &ex) {
        SWIG_exception(SWIG_UnknownError, ex.what().c_str());
    }
}

%nodefaultctor;

%include "yamloi.hpp"
%shared_ptr(yamloi::Node);
%shared_ptr(yamloi::ScalarNode);
%shared_ptr(yamloi::SequenceNode);
%shared_ptr(yamloi::MappingNode);
%template (NodeVector) std::vector<std::shared_ptr<yamloi::Node>>;

%include "Node.hpp"
%include "ScalarNode.hpp"
%include "SequenceNode.hpp"
%include "MappingNode.hpp"
%include "Loader.hpp"



