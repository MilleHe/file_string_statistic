/*#############################################################################
#
#            Notice of Copyright and Non-Disclosure Information
#
#        Nothing, do whatever you like.
#
###############################################################################
*/

#include "common.h"

#include <map>

#include <string.h>


//#############################################################################
#define SERVICE_NAME                                      "File String Statistic"

#define DEFAULT_FILE_PATH                                 "./2701-0.txt"
#define MAX_LINE_LENGTH                                   256

#define DEFAULT_COUNTER                                   20
#define COUNTER_THREASHOLD                                100



//#############################################################################
void statistic(const std::string& s_file, const uint8_t& u_counter)
  {
  typedef std::map<std::string, uint64_t>         statistic_map;
  typedef std::multimap<uint64_t, std::string>    ordered_statistic_map;

  FILE* f_file = NULL;
  statistic_map map_str_counter;
  ordered_statistic_map ordered_map;

  tprint("Open file '%s'", s_file.data());

  //***************************************************************************
  // open to read file
  f_file = fopen(s_file.data(), "r" );
  if (f_file == NULL)
    {
    tprint("Fail to open file '%s'", s_file.data());
    return;
    }

  tprint("File '%s' opened", s_file.data());

  //***************************************************************************
  // read line by line from file
  char c_line[MAX_LINE_LENGTH] = {0};
  bool head_record = false;
  uint8_t str_head_pos = 0;

  // read line
  while (fgets(c_line, MAX_LINE_LENGTH, f_file))
    {
    const uint8_t len = strlen(c_line);
    for (uint8_t itr = 0; itr < len; itr++)
      {
      // is char alphanumeric
      if ((c_line[itr] >= '0' && c_line[itr] <= '9') ||
          (c_line[itr] >= 'A' && c_line[itr] <= 'Z') ||
          (c_line[itr] >= 'a' && c_line[itr] <= 'z'))
        {
        if (!head_record)
          {
          head_record = true;
          str_head_pos = itr;
          }

        continue;
        }

      if (head_record)
        {
        // a word or number, mark string end '\0'
        c_line[itr] = 0;
        head_record = false;

        // count
        map_str_counter[std::string(c_line + str_head_pos)]++;
        }
      }

    memset(c_line, 0, len);
    }

  //***************************************************************************
  // statistic map ready, shrink into ordered container
  while (!map_str_counter.empty())
    {
    const statistic_map::const_iterator& itr = map_str_counter.begin();
    ordered_map.insert(std::make_pair(itr->second, itr->first));

    // constrain size of ordered container as needed
    if (ordered_map.size() > u_counter)
      ordered_map.erase(ordered_map.begin());

    map_str_counter.erase(itr);
    }

  //***************************************************************************
  // output
  for (ordered_statistic_map::const_reverse_iterator ritr = ordered_map.rbegin();
       ritr != ordered_map.rend(); ritr++)
    {
    tprint(" %6ld %s", ritr->first, ritr->second.data());
    }

  // close file handle
  tprint("Close file '%s'", s_file.data());
  fclose(f_file);
  }



//#############################################################################
int main(int argc, char** argv)
  {
  bool is_help = false;
  std::string s_file("");
  uint8_t u_counter = 0;

  //***************************************************************************
  // parameter parse
  for (uint8_t i = 1; i < argc; i++)
    {
    if (std::string(argv[i]) == "-h")
      {
      is_help = true;
      }
    else if (std::string(argv[i]).find("-f") == 0)
      {
      try
        {
        s_file = std::string(argv[i]).substr(2);
        }
      catch(int err)
        {
        is_help = true;
        }
      }
    else if (std::string(argv[i]).find("-c") == 0)
      {
      try
        {
        u_counter = std::stoi(std::string(argv[i]).substr(2).data());
        }
      catch(int err)
        {
        is_help = true;
        }
      }
    }

  if (is_help)
    {
    printf("-h Help\n"
      "-f File path. eg. '-f/tmp/tmp.file\n"
      "   If not provided, default file path is '%s'.\n"
      "-c Count of most used words for be listed in file, eg. '-c10'\n"
      "   Default %d, maximum 100\n",
        DEFAULT_FILE_PATH, DEFAULT_COUNTER);
    return 0;
    }

  // modify counter
  if (u_counter == 0)
    {
    u_counter = DEFAULT_COUNTER;
    }
  else if (u_counter > COUNTER_THREASHOLD)
    {
    u_counter = COUNTER_THREASHOLD;
    }

  // Use default file if not assigned
  if (s_file.empty())
    {
    tprint("No file path assigned, use default '%s'", DEFAULT_FILE_PATH);
    s_file = DEFAULT_FILE_PATH;
    }

  tprint("'%s' starts with file '%s'", SERVICE_NAME, s_file.data());

  // start each thread for each file
  statistic(s_file, u_counter);

  tprint("'%s' ends", SERVICE_NAME);

  return 0;
  }
