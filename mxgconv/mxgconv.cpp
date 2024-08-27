//           ///          //                                  C++ Cross Platform
//          /////        ////
//         /// XXX     XXX ///            ///////////   /////////     ///   ///
//        ///    XXX XXX    ///         ///             ///    ///   ///  ///
//       ///       XXX       ///         /////////     ///      //  //////
//      ///      XXX XXX      ///               ///   ///    ///   ///  ///
//     ////    XXX     XXX    ////    ////////////  //////////    ///   ///
//    ////                     ////
//   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
//  //// Security Vision Systems //////////////////////////////////////////////
//
//  $Author: khe_admin $
//  $LastChangedBy: khe_admin $
//  $LastChangedDate: 2007-06-29 15:34:53 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/mxgconv/trunk/mxgconv.cpp $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licensed under the BSD licence,
     //  http://www.opensource.org/licenses/bsd-license.php
     //
     //  Copyright (c) 2005 - 2007, MOBOTIX AG
     //  All rights reserved.
     //
     //  Redistribution and use in source and binary forms, with or without
     //  modification, are permitted provided that the following conditions are
     //  met:
     //
     //  - Redistributions of source code must retain the above copyright
     //    notice, this list of conditions and the following disclaimer.
     //
     //  - Redistributions in binary form must reproduce the above copyright
     //    notice, this list of conditions and the following disclaimer in the
     //    documentation and/or other materials provided with the distribution.
     //
     //  - Neither the name of MOBOTIX AG nor the names of its contributors may
     //    be used to endorse or promote products derived from this software
     //    without specific prior written permission.
     //
     //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     //  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     //  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     //  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     //
///////



#ifndef MX_PLATFORM_WINDOWS
  #include <unistd.h>
#else
  #define STDIN_FILENO 0
  #define STDOUT_FILENO 1
  #define STDERR_FILENO 2
#endif



#include <mxpeg_sdk/static_component_bootstrap.h>
                                    // for statically linked MxPEG SDK demo case

#include <interfaces/IStreamSourceFile.h>
#include <interfaces/IMxPEGParser.h>
#include <interfaces/IMxPEGScanDecoder.h>
#include <interfaces/IMxPEGScanToJPEGConverter.h>
#include <interfaces/IMxPEGRawFrameDumper.h>
#include <interfaces/IFileWriter.h>

#include <Framework/Framework.h>

#include <mxm/core/mxmApplication.h>
#include <mxm/core/mxmNopStatusMessageHandler.h>
#include <mxm/core/mxmPerlStyleHash.h>
#include <mxm/core/mxm_smart.h>
#include <mxm/core/mxm_generic_stuff.h>

#include <cstdio>
#include <cstdlib>



int dump_usage(const mxmString &prog_name) {
   
   std::puts("MxPEG stream converter"
              ", (c) 2005-2007 MOBOTIX Security Vision Systems");
   std::puts((mxmString("USAGE: ")
               + prog_name
               + " arg1=val1,arg2=val2,...").text());
   std::puts("   if                        : input file (- is stdin, default)");
   std::puts("   of                        : output file without extension");
   std::puts("                               (- is stdout, default)");
   std::puts("   format");
   std::puts("      mjpg                   : Motion-JPEG (default)");
   std::puts("      jpeg                   : individual JPEG images");
   std::puts("      rgb, yuv               : individual raw RGB or YUV (yv12) "
                                             "frames");
   std::puts("      rgb_stream, yuv_stream : raw RGB or YUV (yv12) stream");
   std::puts("   file_num                  : maximum number of output files");
   std::puts("                               (for jpeg, rgb, yuv formats)");
   std::puts("   separator                 : separator string "
                                             "(for mjpg, *_stream formats)");
   std::puts("EXAMPLES:");
   std::puts("   mxgconv if=test_data.mxg,of=another_test,format=jpeg");
   std::puts("   curl <url> | mxgconv >another_test.mjpg");
   
   return(1);
}


int list_components() {
   
   mxm::smart<mxmStringList>
    component_ids = mx::Framework::framework()->enumerateComponents();
   int num_components = mx::Framework::framework()->numComponents(),
       num_anonymous  = num_components - component_ids->size();
   
   if(component_ids->size())
    std::puts(("   " + component_ids->concatenate("\n   ")).text());
   std::puts((mxmString(num_components)
               + " components installed, "
               + num_anonymous + " anonymous").text());
    
   return(0);
}



int main(int argc, char **argv) {
   
   mxm::addStatusMessageHandler(new mxmNopStatusMessageHandler());
                       // make sure we won't get mxm status messages on _stdout_
   
   mxmApplication mxm_app(argc, argv);
   mx::Framework mx_framework(mxpeg_sdk::createStaticallyLinkedComponents());
           // this demo program uses the simple statically linked MxPEG SDK case
   
   mxmString prog_name = argv[0];   // TODO...
   
   // get arguments...
   if(argc > 2) return(dump_usage(prog_name));
   mxmPerlStyleHash args;
   if(argc == 2) args = mxmPerlStyleHash::fromCommandLineArg(argv[1]);
   if(   !args["help"].isNull()
      || !args["--help"].isNull())
    return(dump_usage(prog_name));
   if(   !args["list_components"].isNull()
      || !args["--list_components"].isNull())
    return(list_components());
   if(!args["if"].length()) args["if"] = "-";
   if(!args["of"].length()) args["of"] = "-";
   
   // set up input...
   mx::IStreamSourceFile *source
     = dynamic_cast<mx::IStreamSourceFile *>
        (mx::Framework::newComponentInstance(MX_ISTREAMSOURCEFILE_ID));
   if(args["if"] == "-")
    source->setFileDescriptor(STDIN_FILENO);
   else
    source->setFilename(args["if"]);
   source->activate();
   
   // set up output...
   mx::IFileWriter *writer
     = dynamic_cast<mx::IFileWriter *>
        (mx::Framework::newComponentInstance(MX_IFILEWRITER_ID));
   
   // configure processing...
   
   int file_num = 0;
   if(args["file_num"].length()) args["file_num"].toInt(file_num);
   if(file_num < 0) file_num = 0;
   
   mx::IUndecodedMxPEGFrameReceiver *frame_receiver;
   mxmString format = args["format"],
             outfile_extension;
   if(   (format == "rgb")
      || (format == "rgb_stream")
      || (format == "yuv")
      || (format == "yuv_stream")) {
      
      mx::IMxPEGRawFrameDumper
       *dumper = dynamic_cast<mx::IMxPEGRawFrameDumper *>
                  (mx::Framework
                   ::newComponentInstance(MX_IMXPEGRAWFRAMEDUMPER_ID));
      dumper->setSegmentedStreamReceiver(writer);
      
      mx::IMxPEGScanDecoder
       *scan_decoder = dynamic_cast<mx::IMxPEGScanDecoder *>
                        (mx::Framework
                          ::newComponentInstance(MX_IMXPEGSCANDECODER_ID));
      scan_decoder->setMxPEGTileReceiver(dumper);
      frame_receiver = scan_decoder;
      
      if(!format.subString("_stream"))
       writer->writeIndividualFiles(file_num);
      
      if(format.subString("rgb")) {
         
         outfile_extension = ".rgb";
      }
      else {
         
         dumper->enableYUVDumps(true);
         
         outfile_extension = ".yv12";
      }
   }
   else {
      
      mx::IMxPEGScanToJPEGConverter
       *converter = dynamic_cast<mx::IMxPEGScanToJPEGConverter *>
                     (mx::Framework
                       ::newComponentInstance(MX_IMXPEGSCANTOJPEGCONVERTER_ID));
      converter->setSegmentedStreamReceiver(writer);
      frame_receiver = converter;
      
      if(format == "jpeg") {
         
         writer->writeIndividualFiles(file_num);
         
         outfile_extension = ".jpg";
      }
      else {
         
         outfile_extension = ".mjpg";
      }
   }
   
   if(args["separator"].length())
    writer->setSegmentSeparator(args["separator"]);
   
   if(args["of"] == "-")
    writer->setFileDescriptor(STDOUT_FILENO);
   else
    writer->setFilename(args["of"] + outfile_extension);
   
   mxm::smart<mx::IMxPEGParser>
    parser = mx::Framework::getComponent(MX_IMXPEGPARSER_ID);
   parser->setStreamSource(source);
   parser->setUndecodedMxPEGFrameReceiver(frame_receiver);
   
   // codec configuration is set up!
   // note that all allocated resources are now covered by RAII - we won't have
   // to do any explicit cleanup later...
   
   // do processing...
   while(   parser->sourceStillUp()
         && !writer->errorState())
    parser->processStreamBytes(2048);
   
   writer->shutdown();
   
   // we're done, smart pointer will handle clean-up...
   
   if(writer->errorState()) {
      
      std::fprintf(stderr, "*** failed!\n");
      std::exit(2);
   }
   else {
      
      std::exit(0);
   }
}
