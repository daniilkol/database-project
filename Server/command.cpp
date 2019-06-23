#include "command.h"

int command::init_command (char *string)
{
  char *words[MAX_LENGTH];
  char tab[2];
  tab[0] = char (9);
  change (string, "(", " ");
  change (string, ")", " ");
  change (string, ",", " ");
  change (string, ";", " ");
  change (string, "\n", " ");
  change (string, tab, " ");
  if (string[strlen (string) - 1] == '\n')
    {
      string[strlen (string) - 1] = 0;
    }
  int num_of_words = parse (string, " ", words);
  if (!strcmp (words[0], "quit"))
    {
      type = QUIT;
      return 0;
    }
  else if (!strcmp (words[0], "stop"))
    {
      type = STOP;
      return -1;
    }
  else if (!strcmp (words[0], "insert"))
    {
      type = INSERT;
      set_name (words[1]);
      int new_phone, new_group;
      if ((new_phone = atoi (words[2])) <= 0)
        {
          return -11;
        }
      if ((new_group = atoi (words[3])) <= 0)
        {
          return -12;
        }
      set_group (new_group);
      set_phone (new_phone);
      return 1;
    }
  else if (!strcmp (words[0], "select"))
    {
      type = SELECT;
      int idx_of_word = 1;

      for (int i = 0; i < 4; i++)
        {
          select_order[i] = 0;
        }
      while (idx_of_word < num_of_words && strcmp (words[idx_of_word], "where"))
        {
          if (!strcmp (words[idx_of_word], "*"))
            {
              strcpy (select_order, "NPG");
            }
          else if (!strcmp (words[idx_of_word], "phone"))
            {
              if (!select_order[0]) select_order[0] = 'P';
              else if (!select_order[1]) select_order[1] = 'P';
              else if (!select_order[2]) select_order[2] = 'P';
            }
          else if (!strcmp (words[idx_of_word], "group"))
            {
              if (!select_order[0]) select_order[0] = 'G';
              else if (!select_order[1]) select_order[1] = 'G';
              else if (!select_order[2]) select_order[2] = 'G';
            }
          else if (!strcmp (words[idx_of_word], "name"))
            {
              if (!select_order[0]) select_order[0] = 'N';
              else if (!select_order[1]) select_order[1] = 'N';
              else if (!select_order[2]) select_order[2] = 'N';
            }
          else
            {
              return -22;
            }
          idx_of_word++;
        }

      if (strcmp (words[idx_of_word], "where"))
        {
          return -21;
        }

      idx_of_word++;

      if (make_conditions (words + idx_of_word, num_of_words - idx_of_word) < 0)
        {
          return -23;
        }
      return 1;
    }
  else if (!strcmp (words[0], "delete"))
    {
      type = DELETE;
      if (num_of_words == 1)
        {
          return 3;
        }
      if (strcmp (words[1], "where"))
        {
          return -32;
        }
      if (make_conditions (words + 2, num_of_words - 2) < 0)
        {
          return -31;
        }
      return 1;
    }
  else if (strcmp (words[0], ""))
    {
      return -100;
    }
  return -100;
}

int command::make_conditions (char **words, int num_of_words)
{
  int idx_of_word = 0, new_phone = 0, new_group = 208;
  while (idx_of_word < num_of_words)
    {
      switch (idx_of_word % 4)
        {
        case (0):
          {
            if (strcmp (words[idx_of_word], "name") && strcmp (words[idx_of_word], "phone") && strcmp (words[idx_of_word], "group"))
              {
                return -1;
              }
            if (!strcmp (words[idx_of_word + 1], "like") && (strcmp (words[idx_of_word], "name")))
              {
                return -2;
              }
            condition_type *condition;
            if (!strcmp (words[idx_of_word], "name"))
              {
                set_name (words[idx_of_word + 2]);
                condition = &c_name;
              }
            else if (!strcmp (words[idx_of_word], "group"))
              {
                if ((new_group = atoi (words[idx_of_word + 2])) <= 0)
                  {
                    return -3;
                  }
                set_group (new_group);
                condition = &c_group;
              }
            else if (!strcmp (words[idx_of_word], "phone"))
              {
                if ((new_phone = atoi (words[idx_of_word + 2])) <= 0)
                  {
                    return -3;
                  }
                set_phone (new_phone);
                condition = &c_phone;
              }
            else
              {
                return -3;
              }

            if (!put_condition (condition, words[idx_of_word + 1]))
              {
                return -4;
              }
            idx_of_word += 3;
            break;
          }
        case (3):
          {
            if (!strcmp (words[idx_of_word], "and"))
              {
                operation = AND;
              }
            else if (!strcmp (words[idx_of_word], "or"))
              {
                operation = OR;
              }
            else
              {
                return -3;
              }
            idx_of_word++;
            break;
          }
        }
    }
  return 0;
}

int put_condition (condition_type *condition, char *word)
{
  if (!strcmp (word, "<"))
    {
      *condition = LT;
      return 1;
    }
  if (!strcmp (word, "<="))
    {
      *condition = LE;
      return 1;
    }
  if (!strcmp (word, ">="))
    {
      *condition = GE;
      return 1;
    }
  if (!strcmp (word, ">"))
    {
      *condition = GT;
      return 1;
    }
  if (!strcmp (word, "="))
    {
      *condition = EQ;
      return 1;
    }
  if (!strcmp (word, "<>"))
    {
      *condition = NE;
      return 1;
    }
  if (!strcmp (word, "like"))
    {
      *condition = LIKE;
      return 1;
    }
  return 0;
}

bool suitable_for_conditions (command *cmd, record_node *student)
{
  int bool_name = 0, bool_group = 0, bool_phone = 0;
  switch (cmd->get_condition_for_name())
    {
    case (LE):
      if (strcmp (student->get_name(), cmd->get_name()) > 0)
        {
          bool_name = -1;
        }
      break;
    case (LT):
      if (strcmp (student->get_name(), cmd->get_name()) >= 0)
        {
          bool_name = -1;
        }
      break;
    case (GE):
      if (strcmp (student->get_name(), cmd->get_name()) < 0)
        {
          bool_name = -1;
        }
      break;
    case (GT):
      if (strcmp (student->get_name(), cmd->get_name()) <= 0)
        {
          bool_name = -1;
        }
      break;
    case (EQ):
      if (strcmp (student->get_name(), cmd->get_name()))
        {
          bool_name = -1;
        }
      break;
    case (NE):
      if (!strcmp (student->get_name(), cmd->get_name()))
        {
          bool_name = -1;
        }
      break;
    case (LIKE):
      if (!like (cmd->get_name(), student->get_name()))
        {
          bool_name = -1;
        }
      break;
    default:
      break;
    }

  switch (cmd->get_condition_for_phone())
    {
    case (LE):
      if (student->get_phone () > cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    case (LT):
      if (student->get_phone () >= cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    case (GE):
      if (student->get_phone () < cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    case (GT):
      if (student->get_phone () <= cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    case (EQ):
      if (student->get_phone () != cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    case (NE):
      if (student->get_phone () == cmd->get_phone())
        {
          bool_phone = -1;
        }
      break;
    default:
      break;
    }

  switch (cmd->get_condition_for_group())
    {
    case (LE):
      if (student->get_group () > cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    case (LT):
      if (student->get_group () >= cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    case (GE):
      if (student->get_group () < cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    case (GT):
      if (student->get_group () <= cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    case (EQ):
      if (student->get_group () != cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    case (NE):
      if (student->get_group () == cmd->get_group())
        {
          bool_group = -1;
        }
      break;
    default:
      break;
    }

  if (cmd->get_operation_type () == OR)
    {
      if ((cmd->get_condition_for_name() != COND_NONE && !bool_name) ||
          (cmd->get_condition_for_phone() != COND_NONE && !bool_phone) ||
          (cmd->get_condition_for_group() != COND_NONE && !bool_group))
        {
          return 1;
        }
      else
        {
          return 0;
        }
    }

  else
    {
      if (!bool_name && !bool_group && !bool_phone)
        {
          return 1;
        }
      else
        {
          return 0;
        }
    }
}

void command::print_command (int sock)
{
  char answer[MAX_LENGTH];
  answer[0] = 0;
  switch (type)
    {
    case (SELECT):
      {
        strcat (answer, "select ");
        if (select_order[0] == 'N' && select_order[1] == 'P' && select_order[2] == 'G')
          {
            strcat (answer, "*");
          }
        else
          {
            if (select_order[0] == 'N')
              strcat (answer, "name");
            if (select_order[0] == 'G')
              strcat (answer, "group");
            if (select_order[0] == 'P')
              strcat (answer, "phone");
            if (select_order[1] == 'N')
              strcat (answer, ", name");
            if (select_order[1] == 'G')
              strcat (answer, ", group");
            if (select_order[1] == 'P')
              strcat (answer, ", phone");
            if (select_order[2] == 'N')
              strcat (answer, ", name");
            if (select_order[2] == 'G')
              strcat (answer, ", group");
            if (select_order[2] == 'P')
              strcat (answer, ", phone");
          }
        strcat (answer, " where ");
        if (c_name != COND_NONE)
          {
            strcat (answer, "name ");
            switch (c_name)
              {
              case (LE):
                strcat (answer, "<= ");
                break;
              case (LT):
                strcat (answer, "< ");
                break;
              case (GE):
                strcat (answer, ">= ");
                break;
              case (GT):
                strcat (answer, "> ");
                break;
              case (EQ):
                strcat (answer, "= ");
                break;
              case (NE):
                strcat (answer, "<> ");
                break;
              case (LIKE):
                strcat (answer, "like ");
                break;
              default:
                break;
              }
            strcat (answer, get_name());
          }
        if (c_name != COND_NONE && operation == OR)
          {
            strcat (answer, " or ");
          }
        if (c_name != COND_NONE && operation == AND)
          {
            strcat (answer, " and ");
          }
        if (c_phone != COND_NONE)
          {
            strcat (answer, "phone ");
            switch (c_phone)
              {
              case (LE):
                strcat (answer, "<= ");
                break;
              case (LT):
                strcat (answer, "< ");
                break;
              case (GE):
                strcat (answer, ">= ");
                break;
              case (GT):
                strcat (answer, "> ");
                break;
              case (EQ):
                strcat (answer, "= ");
                break;
              case (NE):
                strcat (answer, "<> ");
                break;
              default:
                break;
              }
            char phone_str[8];
            phone_str[0] = 0;
            itoa (get_phone (), phone_str);
            strcat (answer, phone_str);
          }
        if (c_name == COND_NONE && c_phone != COND_NONE && operation == OR)
          {
            strcat (answer, " or ");
          }
        if (c_name == COND_NONE && c_phone != COND_NONE && operation == AND)
          {
            strcat (answer, " and ");
          }
        if (c_group != COND_NONE)
          {
            strcat (answer, "group ");
            switch (c_group)
              {
              case (LE):
                strcat (answer, "<= ");
                break;
              case (LT):
                strcat (answer, "< ");
                break;
              case (GE):
                strcat (answer, ">= ");
                break;
              case (GT):
                strcat (answer, "> ");
                break;
              case (EQ):
                strcat (answer, "= ");
                break;
              case (NE):
                strcat (answer, "<> ");
                break;
              default:
                break;
              }
            char group_str[4];
            group_str[0] = 0;
            itoa (get_group (), group_str);
            strcat (answer, group_str);
          }
        strcat (answer, ";");
        write_fd (sock, answer);
        break;
      }
    case (INSERT):
      char _str[8];
      strcat (answer, "insert (");
      strcat (answer, get_name ());
      strcat (answer, " ");
      _str[0] = 0;
      itoa (get_phone (), _str);
      strcat (answer, _str);
      strcat (answer, " ");
      _str[0] = 0;
      itoa (get_group (), _str);
      strcat (answer, _str);
      strcat (answer, ");");
      write_fd (sock, answer);
      break;
    case (DELETE):
      strcat (answer, "delete");
      if (get_name() || get_phone() ||  get_group())
        {
          strcat (answer, " where ");
          if (c_name != COND_NONE)
            {
              strcat (answer, "name ");
              switch (c_name)
                {
                case (LE):
                  strcat (answer, "<= ");
                  break;
                case (LT):
                  strcat (answer, "< ");
                  break;
                case (GE):
                  strcat (answer, ">= ");
                  break;
                case (GT):
                  strcat (answer, "> ");
                  break;
                case (EQ):
                  strcat (answer, "= ");
                  break;
                case (NE):
                  strcat (answer, "<> ");
                  break;
                case (LIKE):
                  strcat (answer, "like ");
                  break;
                default:
                  break;
                }
              strcat (answer, get_name());
            }
          if (c_name != COND_NONE && operation == OR)
            {
              strcat (answer, " or ");
            }
          if (c_name != COND_NONE && operation == AND)
            {
              strcat (answer, " and ");
            }
          if (c_phone != COND_NONE)
            {
              strcat (answer, "phone ");
              switch (c_phone)
                {
                case (LE):
                  strcat (answer, "<= ");
                  break;
                case (LT):
                  strcat (answer, "< ");
                  break;
                case (GE):
                  strcat (answer, ">= ");
                  break;
                case (GT):
                  strcat (answer, "> ");
                  break;
                case (EQ):
                  strcat (answer, "= ");
                  break;
                case (NE):
                  strcat (answer, "<> ");
                  break;
                default:
                  break;
                }
              char phone_str[8];
              phone_str[0] = 0;
              itoa (get_phone (), phone_str);
              strcat (answer, phone_str);
            }
          if (c_name == COND_NONE && c_phone != COND_NONE && operation == OR)
            {
              strcat (answer, " or ");
            }
          if (c_name == COND_NONE && c_phone != COND_NONE && operation == AND)
            {
              strcat (answer, " and ");
            }
          if (c_group != COND_NONE)
            {
              strcat (answer, "group ");
              switch (c_group)
                {
                case (LE):
                  strcat (answer, "<= ");
                  break;
                case (LT):
                  strcat (answer, "< ");
                  break;
                case (GE):
                  strcat (answer, ">= ");
                  break;
                case (GT):
                  strcat (answer, "> ");
                  break;
                case (EQ):
                  strcat (answer, "= ");
                  break;
                case (NE):
                  strcat (answer, "<> ");
                  break;
                default:
                  break;
                }
              char group_str[4];
              group_str[0] = 0;
              itoa (get_group (), group_str);
              strcat (answer, group_str);
            }
          strcat (answer, ";");
        }
      else
        {
          strcat (answer, ";");
        }
      write_fd (sock, answer);
      break;
    case (QUIT):
      write_fd (sock, "quit;");
      break;
    default:
      break;
    }
}


void command::print_command (FILE **output)
{
    switch (type)
    {
    case (SELECT):
    {
        fprintf (*output, "select ");
        if (select_order[0] == 'N' && select_order[1] == 'P' && select_order[2] == 'G')
        {
            fprintf (*output, "*");
        }
        else
        {
        if (select_order[0] == 'N')
            fprintf (*output, "name");
        if (select_order[0] == 'G')
            fprintf (*output, "group");
        if (select_order[0] == 'P')
            fprintf (*output, "phone");
        if (select_order[1] == 'N')
            fprintf (*output, ", name");
        if (select_order[1] == 'G')
            fprintf (*output, ", group");
        if (select_order[1] == 'P')
            fprintf (*output, ", phone");
        if (select_order[2] == 'N')
            fprintf (*output, ", name");
        if (select_order[2] == 'G')
            fprintf (*output, ", group");
        if (select_order[2] == 'P')
            fprintf (*output, ", phone");
        }
        fprintf (*output, " where ");
        if (c_name != COND_NONE)
        {
            fprintf (*output, "name ");
            switch (c_name)
            {
            case (LE):
                fprintf (*output, "<=");
                break;
            case (LT):
                fprintf (*output, "<");
                break;
            case (GE):
                fprintf (*output, ">=");
                break;
            case (GT):
                fprintf (*output, ">");
                break;
            case (EQ):
                fprintf (*output, "=");
                break;
            case (NE):
                fprintf (*output, "<>");
                break;
            case (LIKE):
                fprintf (*output, "like");
                break;
            default:
                break;
            }
            fprintf (*output, " %s", get_name());
        }
        if (c_name != COND_NONE && operation == OR)
        {
            fprintf (*output, " or ");
        }
        if (c_name != COND_NONE && operation == AND)
        {
            fprintf (*output, " and ");
        }
        if (c_phone != COND_NONE)
        {
            fprintf (*output, "phone ");
            switch (c_phone)
            {
            case (LE):
                fprintf (*output, "<=");
                break;
            case (LT):
                fprintf (*output, "<");
                break;
            case (GE):
                fprintf (*output, ">=");
                break;
            case (GT):
                fprintf (*output, ">");
                break;
            case (EQ):
                fprintf (*output, "=");
                break;
            case (NE):
                fprintf (*output, "<>");
                break;
            default:
                break;
            }
            fprintf (*output, " %d", get_phone());
        }
        if (c_name == COND_NONE && c_phone != COND_NONE && operation == OR)
        {
            fprintf (*output, " or ");
        }
        if (c_name == COND_NONE && c_phone != COND_NONE && operation == AND)
        {
            fprintf (*output, " and ");
        }
        if (c_group != COND_NONE)
        {
            fprintf (*output, "group ");
            switch (c_group)
            {
            case (LE):
                fprintf (*output, "<=");
                break;
            case (LT):
                fprintf (*output, "<");
                break;
            case (GE):
                fprintf (*output, ">=");
                break;
            case (GT):
                fprintf (*output, ">");
                break;
            case (EQ):
                fprintf (*output, "=");
                break;
            case (NE):
                fprintf (*output, "<>");
                break;
            default:
                break;
            }
            fprintf (*output, " %d", get_group());
        }
        fprintf (*output, ";\n");
        break;
    }
    case (INSERT):
        fprintf (*output, "insert ");
        fprintf (*output, "(%s, %d, %d);\n", get_name(), get_phone(), get_group());
        break;
    case (DELETE):
        fprintf (*output, "delete");
        if (get_name() || get_phone() ||  get_group())
        {
            fprintf (*output, " where ");
            if (c_name != COND_NONE)
            {
                fprintf (*output, "name ");
                switch (c_name)
                {
                case (LE):
                    fprintf (*output, "<=");
                    break;
                case (LT):
                    fprintf (*output, "<");
                    break;
                case (GE):
                    fprintf (*output, ">=");
                    break;
                case (GT):
                    fprintf (*output, ">");
                    break;
                case (EQ):
                    fprintf (*output, "=");
                    break;
                case (NE):
                    fprintf (*output, "<>");
                    break;
                case (LIKE):
                    fprintf (*output, "like");
                    break;
                default:
                    break;
                }
                fprintf (*output, " %s", get_name());
            }
            if (c_name != COND_NONE && operation == OR)
            {
                fprintf (*output, " or ");
            }
            if (c_name != COND_NONE && operation == AND)
            {
                fprintf (*output, " and ");
            }
            if (c_phone != COND_NONE)
            {
                fprintf (*output, "phone ");
                switch (c_phone)
                {
                case (LE):
                    fprintf (*output, "<=");
                    break;
                case (LT):
                    fprintf (*output, "<");
                    break;
                case (GE):
                    fprintf (*output, ">=");
                    break;
                case (GT):
                    fprintf (*output, ">");
                    break;
                case (EQ):
                    fprintf (*output, "=");
                    break;
                case (NE):
                    fprintf (*output, "<>");
                    break;
                default:
                    break;
                }
                fprintf (*output, " %d", get_phone());
            }
            if (c_name == COND_NONE && c_phone != COND_NONE && operation == OR)
            {
                fprintf (*output, " or ");
            }
            if (c_name == COND_NONE && c_phone != COND_NONE && operation == AND)
            {
                fprintf (*output, " and ");
            }
            if (c_group != COND_NONE)
            {
                fprintf (*output, "group ");
                switch (c_group)
                {
                case (LE):
                    fprintf (*output, "<=");
                    break;
                case (LT):
                    fprintf (*output, "<");
                    break;
                case (GE):
                    fprintf (*output, ">=");
                    break;
                case (GT):
                    fprintf (*output, ">");
                    break;
                case (EQ):
                    fprintf (*output, "=");
                    break;
                case (NE):
                    fprintf (*output, "<>");
                    break;
                default:
                    break;
                }
                fprintf (*output, " %d", get_group());
            }
            fprintf (*output, ";\n");

        }
        else
            fprintf (*output, ";\n");
        break;
    case (QUIT):
        fprintf (*output, "quit;\n");
        break;
    default:
        break;
    }
}
