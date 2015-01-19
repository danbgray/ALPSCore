/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <alps/params.hpp>
#include "gtest/gtest.h"
#include <alps/utilities/temporary_filename.hpp>
#include <fstream>

// Scalar param read
TEST(param, ScalarParamRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   int param_int=1234;
   double param_double=1.125;
   float param_float=2.25;
   bool param_true=true;
   bool param_false=false;
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << std::boolalpha;
     pfile << "int = " << param_int << std::endl;
     pfile << "double = " << param_double << std::endl;
     pfile << "float = " << param_float << std::endl;
     pfile << "partrue = " << param_true << std::endl;
     pfile << "parfalse = " << param_false << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("int","int parameter").
       define<double>("double","double parameter").
       define<float>("float","float parameter").
       define<bool>("partrue","partrue parameter").
       define<bool>("parfalse","parfalse parameter");
   
   // read the parameters
   
   int param_int_rd=p["int"].as<int>();
   double param_double_rd=p["double"].as<double>();
   float param_float_rd=p["float"].as<float>();
   bool param_true_rd=p["partrue"].as<bool>();
   bool param_false_rd=p["parfalse"].as<bool>();

   // verify the parameters
   EXPECT_EQ(param_int_rd, param_int);
   EXPECT_EQ(param_true_rd, param_true);
   EXPECT_EQ(param_false_rd, param_false);
   EXPECT_NEAR(param_double_rd,param_double,1.E-12);
   EXPECT_NEAR(param_float_rd,param_float,1.E-12);
}    

// String param read (stripping spaces, stripping quotes)
TEST(param, StringParamRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

/*     
     basic
     no_tl_spaces
     with_tl_spaces
     with_tl_spaces_quoted
     with_tl_spaces_quoted2
     ends_with_quote
     starts_with_quote
*/


   // Have some sample strings
   std::string basic="hello-world";
   std::string no_tl_spaces="hello, \"my beautiful\" world"; // no trailing/leading spaces
   std::string with_tl_spaces="   " + no_tl_spaces + " ";
   std::string with_tl_spaces_quoted="\"" + with_tl_spaces + "\"";
   std::string with_tl_spaces_quoted2="\"\"" + with_tl_spaces + "\"\"";
   std::string ends_with_quote="Hello, \"my world\"";
   std::string starts_with_quote="\"Hello, world!\", she said.";

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "basic = " << basic << std::endl;
     pfile << "no_tl_spaces = " << no_tl_spaces << std::endl;
     pfile << "with_tl_spaces = " << with_tl_spaces << std::endl;
     pfile << "with_tl_spaces_quoted = " << with_tl_spaces_quoted << std::endl;
     pfile << "with_tl_spaces_quoted2 = " << with_tl_spaces_quoted2 << std::endl;
     pfile << "ends_with_quote = " << ends_with_quote << std::endl;
     pfile << "starts_with_quote = " << starts_with_quote << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<std::string>("basic","basic string parameter").
       define<std::string>("no_tl_spaces","No trailing or leading spaces").
       define<std::string>("with_tl_spaces","with trailing and leading spaces").
       define<std::string>("with_tl_spaces_quoted","with t/l spaces, quoted").
       define<std::string>("with_tl_spaces_quoted2","with t/l spaces/quotes, quoted").
       define<std::string>("ends_with_quote","ending with quote").
       define<std::string>("starts_with_quote","starting with quote");
   
   // read the parameters
   std::string basic_rd=p["basic"].as<std::string>();
   std::string no_tl_spaces_rd=p["no_tl_spaces"].as<std::string>();
   std::string with_tl_spaces_rd=p["with_tl_spaces"].as<std::string>();
   std::string with_tl_spaces_quoted_rd=p["with_tl_spaces_quoted"].as<std::string>();
   std::string with_tl_spaces_quoted2_rd=p["with_tl_spaces_quoted2"].as<std::string>();
   std::string ends_with_quote_rd=p["ends_with_quote"].as<std::string>();
   std::string starts_with_quote_rd=p["starts_with_quote"].as<std::string>();

   // verify the parameters
   EXPECT_EQ(basic_rd, basic);
   EXPECT_EQ(no_tl_spaces_rd, no_tl_spaces);
   EXPECT_EQ(with_tl_spaces_rd, no_tl_spaces);
   EXPECT_EQ(with_tl_spaces_quoted_rd, with_tl_spaces);
   EXPECT_EQ(with_tl_spaces_quoted2_rd, with_tl_spaces_quoted);
   EXPECT_EQ(ends_with_quote_rd,ends_with_quote);
   EXPECT_EQ(starts_with_quote_rd,starts_with_quote);
}    

// Scalars and strings with default values

// Vector param read

// Sectioned ini file

// Various spaces around

// Comments and empty lines

// Incorrect input (garbage lines, wrong-type values)
   
// Incorrect name access (different test file?)

// Assigned vs file-read parameters (different test file?)

#if 0
void Test(void) {
// TEST(param, TextParamRead){
   // define a few vector types
   // typedef alps::params::dblvec dblvec;
   // typedef alps::params::some_vec<int> intvec;
    typedef std::vector<double> dblvec;
    typedef std::vector<int> intvec;
  
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   //create a few scalar parameters
   int int_parameter=1;
   double double_parameter =2.;
   bool bool_parameter_true=true;
   bool bool_parameter_false=false;
   std::string string_parameter="Hello";
   std::string string_parameter_semic="Hello;";
   std::string complicated_string_parameter="/path/to/nowhere -parameter -run --this -program";

   //create some vector parameters
   dblvec dvec_param(3);
   dvec_param[0]=1.25; dvec_param[1]=2.25; dvec_param[2]=4.25; 

   intvec mvec_param(3);
   mvec_param[0]=10; mvec_param[1]=20; mvec_param[2]=30;
   
   {
     std::ofstream pfile(pfilename.c_str());
     pfile<<"INT = "<<int_parameter<<std::endl;
     pfile<<"DOUBLE = "<<double_parameter<<std::endl;
     pfile<<"STRING = "<<string_parameter<<std::endl;
     pfile<<"STRING_SEMI = "<<string_parameter_semic<<std::endl;
     pfile<<"BOOL_TRUE = "<<std::boolalpha<<bool_parameter_true<<std::endl;
     pfile<<"BOOL_FALSE = "<<bool_parameter_false<<std::endl;
     pfile<<"" <<std::endl; //empty line
     pfile<<"# this is a  line with some gunk in it" <<std::endl; //non-parameter line
     pfile<<"complicated_string = "<<complicated_string_parameter<<std::endl;
     pfile<<"NOSPACE_LEFT= "<<double_parameter<<std::endl;
     pfile<<"NOSPACE_RIGHT ="<<double_parameter<<std::endl;
     pfile<<"NOSPACE_BOTH="<<double_parameter<<std::endl;

     pfile << "DVEC=" << dvec_param[0] << ',' << dvec_param[1] << ',' << dvec_param[2] << std::endl;
     pfile << "MVEC=" << mvec_param[0] << ',' << mvec_param[1] << ',' << mvec_param[2] << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
     define<int>("INT","Int parameter").
     define<double>("DOUBLE","Double parameter").
     define<std::string>("STRING","String parameter").
     define<std::string>("STRING_SEMI","String parameter with semicolon").
     define<bool>("BOOL_TRUE","Bool parameter which is true").
     define<bool>("BOOL_FALSE","Bool parameter which is false").
     define<std::string>("complicated_string","complicated string").
     define<double>("NOSPACE_LEFT","parameter in INI without space left of '='").
     define<double>("NOSPACE_RIGHT","parameter in INI without space right of '='").
     define<double>("NOSPACE_BOTH","parameter in INI without spaces around '='").
     define< alps::params::vector<double> >("DVEC","double vector").
     define< alps::params::vector<int> >("MVEC","int vector");
   

   //  read the parameters (parsing on first access)
   int int_parameter_read=p["INT"].as<int>();  
   double double_parameter_read=p["DOUBLE"].as<double>();  
   std::string string_parameter_read=p["STRING"].as<std::string>();
   std::string string_parameter_semi_read=p["STRING_SEMI"].as<std::string>();
   std::string complicated_string_parameter_read=p["complicated_string"].as<std::string>();
   bool bool_parameter_true_read=p["BOOL_TRUE"].as<bool>();
   bool bool_parameter_false_read=p["BOOL_FALSE"].as<bool>();
   double nospace_left=p["NOSPACE_LEFT"].as<double>();  
   double nospace_right=p["NOSPACE_RIGHT"].as<double>();  
   double nospace_both=p["NOSPACE_BOTH"].as<double>();

   dblvec dvec_param_read=p["DVEC"].as<dblvec>();
   intvec mvec_param_read=p["MVEC"].as<intvec>();

   EXPECT_EQ(int_parameter, int_parameter_read);    
   EXPECT_NEAR(double_parameter, double_parameter_read, 1.e-12);    
   EXPECT_EQ(string_parameter, string_parameter_read);    
   EXPECT_EQ(complicated_string_parameter, complicated_string_parameter_read);    
   EXPECT_EQ(string_parameter, string_parameter_semi_read); //test that the ; gets trimmed
   EXPECT_EQ(bool_parameter_true, bool_parameter_true_read); 
   EXPECT_EQ(bool_parameter_false, bool_parameter_false_read); 
   EXPECT_NEAR(double_parameter, nospace_left, 1.e-12);    
   EXPECT_NEAR(double_parameter, nospace_right, 1.e-12);    
   EXPECT_NEAR(double_parameter, nospace_both, 1.e-12);

   EXPECT_EQ(dvec_param.size(),dvec_param_read.size());
   for (int i=0; i<dvec_param.size(); ++i) {
       EXPECT_NEAR(dvec_param[i], dvec_param_read[i], 1.e-12);
   }
   EXPECT_EQ(mvec_param.size(),mvec_param_read.size());
   for (int i=0; i<mvec_param.size(); ++i) {
       EXPECT_EQ(mvec_param[i], mvec_param_read[i]);
   }
}
#endif

int main(int argc, char **argv) 
{
//  Test();
//  return 0;
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
