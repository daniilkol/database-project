#define MAX_LEN 128

#include "strings.h"

struct previous
{
  int idx;
  int shift;
  previous (int idx0, int shift0)
  {
    idx = idx0;
    shift = shift0;
  }
};

int like (const char *examplar, const char *string)
{
  int len_of_ex = strlen (examplar), len_of_str = strlen (string);

  if (!strchr (examplar, '%'))
    {
      if (len_of_ex != len_of_str)
        {
          return 0;
        }
      for (int idx = 0; idx < len_of_ex; idx++)
        {
          if (examplar[idx] != '_' && examplar[idx] != string[idx])
            {
              return 0;
            }
        }
      return 1;
    }

  int start = strchr (examplar, '%') - examplar;
  for (int idx = 0; idx < start; idx++)
    {
      if (examplar[idx] != '_' && examplar[idx] != string[idx])
        {
          return 0;
        }
    }

  std::stack < previous > previous_proc;
  int cur_idx;
  int shift = 0;
  for (cur_idx = start; ; cur_idx++)
    {
      if (cur_idx == len_of_ex)
        {
          if (cur_idx + shift == len_of_str)
            {
              return 1;
            }
          if (examplar[cur_idx - 1] == '%')
            {
              return 1;
            }
          cur_idx = previous_proc.top ().idx;
          shift = previous_proc.top ().shift + 1;
          previous_proc.pop ();
          while (cur_idx + shift >= len_of_str)
            {
              if (previous_proc.empty ())
                {
                  return 0;
                }
              cur_idx = previous_proc.top ().idx;
              shift = previous_proc.top ().shift + 1;
              previous_proc.pop ();
            }
          previous_proc.push (previous (cur_idx, shift));
          continue;
        }
      if (examplar[cur_idx] == '%')
        {
          shift--;
          previous_proc.push (previous (cur_idx, shift));
          continue;
        }
      if (cur_idx + shift >= len_of_str)
        {
          cur_idx = previous_proc.top ().idx;
          shift = previous_proc.top ().shift + 1;
          previous_proc.pop ();
          while (cur_idx + shift >= len_of_str)
            {
              if (previous_proc.empty ())
                {
                  return 0;
                }
              cur_idx = previous_proc.top ().idx;
              shift = previous_proc.top ().shift + 1;
              previous_proc.pop ();
            }
          previous_proc.push (previous (cur_idx, shift));
          continue;
        }
      if (examplar[cur_idx] == '_' || examplar[cur_idx] == string[cur_idx + shift])
        {
          continue;
        }
      else
        {
          cur_idx = previous_proc.top ().idx;
          shift = previous_proc.top ().shift + 1;
          previous_proc.pop ();
          previous_proc.push (previous (cur_idx, shift));
        }
    }
  return 0;
}

int parse (char *string, const char *space, char **words)
{
  char *cur_point = 0;
  int len_of_space = strlen (space);
  int cur_num = 0;
  cur_point = strstr (string, space);
  while (cur_point)
    {
      if (cur_point == string)
        {
          string = string + len_of_space;
          cur_point = strstr (string, space);
          continue;
        }
      words[cur_num] = string;
      words[cur_num][cur_point - string] = 0;
      string = cur_point + len_of_space;
      cur_num++;
      cur_point = strstr (string, space);

    }
  if (string[0])
    {
      words[cur_num++] = string;
    }
  return cur_num;
}

int change (char *string,
	    const char *substr,
	    const char *new_substr)
{
  int id_ = 0,
      len_string = 0,
      len_substr = strlen (substr),
      len_new_substr = strlen (new_substr),
      difference = len_new_substr - len_substr;
  char *pointer;

  if (difference >= 0)
    {
      pointer = strstr(string, substr);
      while (pointer)
        {
          len_string = strlen(string);
          for (id_ = len_string + difference - 1; id_ >= pointer + len_new_substr - string; id_--)
            {
              string[id_] = string[id_ - difference];
            }
          strncpy(pointer, new_substr, len_new_substr);
          pointer = strstr(string, substr);
        }
    }
  else
    {
      pointer = strstr(string, substr);
      while (pointer)
        {
          len_string = strlen(string);
          for (id_ = pointer + len_new_substr - string; id_ < len_string + difference; id_++)
            {
              string[id_] = string[id_ - difference];
            }
          strncpy(pointer, new_substr, len_new_substr);
          string[id_] = 0;
          pointer = strstr(string, substr);
        }
    }
  return 0;
}
