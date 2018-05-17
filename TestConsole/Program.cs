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
            using (FileStream stream = new FileStream(@"c:\users\daviddi\videos\big_buck_bunny_480p_h264.mov", FileMode.Open))
            {
                System.Runtime.InteropServices.ComTypes.IStream comStream = stream.ToComStream();

                FFmpegInteropMSS mss = FFmpegInteropMSS.CreateFFmpegInteropMSSFromStream(comStream, false, false);

                mss.Starting(TimeSpan.FromSeconds(2));
                MyMediaStreamSample sample = mss.SampleRequested(false);

                sample.Dispose();
            }
        }
    }
}
