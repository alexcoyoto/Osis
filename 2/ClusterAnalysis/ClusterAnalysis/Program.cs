using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;
using System.Windows.Forms;

namespace ClusterAnalysis
{
    unsafe class Program
    {
        const uint FSCTL_GET_RETRIEVAL_POINTERS = 0x00090073;

        [StructLayout(LayoutKind.Sequential)]
        struct STARTING_VCN_INPUT_BUFFER
        {
            public static readonly int Size;

            static STARTING_VCN_INPUT_BUFFER()
            {
                Size = Marshal.SizeOf(typeof(STARTING_VCN_INPUT_BUFFER));
            }

            public long StartingVcn;
        }

        [StructLayout(LayoutKind.Sequential)]
        struct RETRIEVAL_POINTERS_BUFFER
        {
            public static readonly int Size;

            static RETRIEVAL_POINTERS_BUFFER()
            {
                Size = Marshal.SizeOf(typeof(RETRIEVAL_POINTERS_BUFFER));
            }

            public int ExtentCount;
            public long StartingVcn;

            public long NextVcn;
            public long Lcn;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool DeviceIoControl(
            SafeFileHandle hFile,
            uint ioctl,
            void* In,
            int InSize,
            void* Out,
            int OutSize,
            int* BytesReturned,
            void* zero
            );

        [STAThread]
        static void Main(string[] args)
        {
            string dir;

            using (var fbd = new FolderBrowserDialog())
            {
                DialogResult result = fbd.ShowDialog(); // окно диалога с пользователем

                if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
                {
                    dir = fbd.SelectedPath;
                    Console.WriteLine("dir: " + dir);

                    string[] files = Directory.GetFiles(dir);
                    long maxClusters = 0;
                    string max_path = "";
                    foreach (string path in files)
                    {
                        Console.WriteLine("File: " + path);
                        using (var file = File.Open(path, FileMode.Open, FileAccess.Read, FileShare.Read))
                        {
                            var vcnIn = new STARTING_VCN_INPUT_BUFFER();
                            var rpb = new RETRIEVAL_POINTERS_BUFFER();
                            int bytesReturned = 0;
                            int err = 0;
                            long totalClusters = 0;
                            int extentNumber = 1;

                            vcnIn.StartingVcn = 0L;

                            do
                            {
                                DeviceIoControl(
                                    file.SafeFileHandle, FSCTL_GET_RETRIEVAL_POINTERS,
                                    &vcnIn, STARTING_VCN_INPUT_BUFFER.Size,
                                    &rpb, RETRIEVAL_POINTERS_BUFFER.Size,
                                    &bytesReturned, null
                                    );

                                err = Marshal.GetLastWin32Error();

                                switch (err)
                                {
                                    case 38: // номер ошибки доступа в память

                                        Console.WriteLine(
                                                "\rОбщее число кластеров файла: {0}\r\nКоличество фрагментов: {1}\n\n",
                                                totalClusters, extentNumber - 1
                                                );

                                        break;
                                    case 0: // без ошибок

                                        Console.WriteLine(
                                            "\tНачальный кластер: {0}\r\n\tДлинна: {1} кластеров",
                                            rpb.Lcn,
                                            rpb.NextVcn - rpb.StartingVcn
                                            );
                                        Console.WriteLine(
                                                "\rОбщее число кластеров файла: {0}\r\nКоличество фрагментов: {1}\n\n",
                                                rpb.NextVcn - rpb.StartingVcn, extentNumber
                                                );
                                        if (rpb.NextVcn - rpb.StartingVcn > maxClusters)
                                        {
                                            maxClusters = rpb.NextVcn - rpb.StartingVcn;
                                            max_path = path;
                                        }

                                        break;
                                    case 234: // ошибка размерности файла

                                        Console.WriteLine(
                                            "\tНачальный кластер: {0}\r\n\tДлинна: {1} кластеров",
                                            rpb.Lcn,
                                            rpb.NextVcn - rpb.StartingVcn
                                            );

                                        totalClusters += rpb.NextVcn - rpb.StartingVcn;
                                        vcnIn.StartingVcn = rpb.NextVcn;

                                        break;
                                    default:
                                        throw new Win32Exception(Marshal.GetLastWin32Error());
                                }

                            } while (err == 234);
                        }
                    }
                    Console.WriteLine("\nМаксимальное количество кластеров: " + maxClusters.ToString() + ";\nПуть: " + max_path);
                }
            }  
            Console.ReadKey();
        }
    }
}
