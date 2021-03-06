﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TempestWpf
{
   /// <summary>
   /// Represents a line in the debug window
   /// </summary>
   public class DebugLine
   {
      private string text;
      private int address;

      /// <summary>
      /// Initializes a new instance of class DebugLine
      /// </summary>
      /// <param name="s">the line's text</param>
      public DebugLine(string s)
      {
         text = s;

         address = -1;
         if (s.Length >= 4)
            address = ParseHex(s.Substring(0, 4));
      }

      /// <summary>
      /// Gets the address associated with the line, -1 if this is not an actual code line
      /// </summary>
      public int Address
      {
         get
         {
            return address;
         }
      }

      /// <summary>
      /// Gets or sets a value indicating whether the object is selected
      /// </summary>
      public bool IsSelected
      {
         get;
         set;
      }

      /// <summary>
      /// Gets the text
      /// </summary>
      public string Text
      {
         get
         {
            return text;
         }
      }

      private int ParseHex(string s)
      {
         int result = 0;
         for (int i = 0; i < s.Length; ++i)
         {
            int digit;
            if (s[i] >= '0' && s[i] <= '9')
               digit = s[i] - '0';
            else if (s[i] >= 'a' && s[i] <= 'f')
               digit = 10 + s[i] - 'a';
            else if (s[i] >= 'A' && s[i] <= 'F')
               digit = 10 + s[i] - 'A';
            else
               return -1;
            result = 16 * result + digit;
         }

         return result;
      }
   }
}
