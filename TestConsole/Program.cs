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
