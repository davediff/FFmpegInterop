// -----------------------------------------------------------------------
//  <copyright file="Program.cs" company="Microsoft Corporation">
//      Copyright (C) Microsoft Corporation. All rights reserved.
//  </copyright>
// -----------------------------------------------------------------------

namespace TestConsole
{
    using System;
    using System.IO;
    using System.Runtime.InteropServices.ComTypes;
    using FFmpegInterop;
    using OneDrive.Media.Streams;

    internal class Program
    {
        private static void Main(string[] args)
        {
            // TODO: Switch back to using streams, for testing file path is hardcoded @ FFmpegInteropMSS.cpp line 306
            using (FileStream stream = new FileStream(@"c:\users\daviddi\downloads\videos\BigBuckBunny_2000h264.mp4", FileMode.Open))
            {
                System.Runtime.InteropServices.ComTypes.IStream comStream = stream.ToComStream();

                FFmpegInteropMSS mss = FFmpegInteropMSS.CreateFFmpegInteropMSSFromStream(comStream, false, false);

                //mss.Starting(TimeSpan.FromSeconds(2));
                mss.SampleRequested(false);

                //sample.Dispose();
            }
        }
    }
}
