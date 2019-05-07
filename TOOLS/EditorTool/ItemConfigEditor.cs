using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorTool
{
    public partial class ItemConfigEditor : Form
    {
        UsefulFunctions useful_functions = new UsefulFunctions();
        JToken controller_config;
        string cam_type = "GREEN_SHELL";
        string position = "1";
        public ItemConfigEditor()
        {
            InitializeComponent();

            //Load config
            controller_config = JToken.Parse(File.ReadAllText("DATA/CONFIGS/ITEM_CONFIG.JSON"));

            //Select item always
            probabilities_position.SelectedIndex = 0;
            itemSelect.SelectedIndex = 0;
        }

        /* Load a new item from the dropdown, first save old config */
        private void itemSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Save current config
            JToken item_info = controller_config[cam_type]["info"];
            saveKeyIfEnabled(item_info, lifetime, "lifetime");
            saveKeyIfEnabled(item_info, player_immunity_time, "player_immunity_time");
            saveKeyIfEnabled(item_info, max_speed, "max_speed");
            saveKeyIfEnabled(item_info, speed, "speed");
            saveKeyIfEnabled(item_info, use_pos_offset, "use_pos_offset");
            saveKeyIfEnabled(item_info, max_bounce_count, "max_bounce_count");
            saveKeyIfEnabled(item_info, collision_frame_delay, "collision_frame_delay");
            saveKeyIfEnabled(item_info, move_toward_distance_squared, "move_toward_distance_squared");
            saveKeyIfEnabled(item_info, duration_multiplier, "duration_multiplier");
            saveKeyIfEnabled(item_info, boost_amount, "boost", "amount");
            saveKeyIfEnabled(item_info, boost_min_velocity, "boost", "min_velocity");
            saveKeyIfEnabled(item_info, boost_duration, "boost", "duration");
            saveKeyIfEnabled(item_info, boost_player_drag, "boost", "player_drag");
            saveKeyIfEnabled(item_info, boost_move_speed, "boost", "move_speed");
            saveKeyIfEnabled(item_info, boost_turn_speed, "boost", "turn_speed");
            saveKeyIfEnabled(item_info, explosion_size_multiplier, "explosion", "size_multiplier");
            saveKeyIfEnabled(item_info, explosion_growth_duration, "explosion", "growth_duration");
            saveKeyIfEnabled(item_info, throw_horizontal_pos_offset, "throw", "horizontal_pos_offset");
            saveKeyIfEnabled(item_info, throw_vertical_pos_offset, "throw", "vertical_pos_offset");
            saveKeyIfEnabled(item_info, throw_forward_force, "throw", "forward_force");
            saveKeyIfEnabled(item_info, throw_upward_force, "throw", "upward_force");
            saveKeyIfEnabled(item_info, throw_max_gravity, "throw", "max_gravity");
            saveKeyIfEnabled(item_info, spin_revolutions, "spin", "revolutions");
            saveKeyIfEnabled(item_info, spin_duration, "spin", "duration");
            saveKeyIfEnabled(item_info, ai_move_speed, "ai", "move_speed");
            saveKeyIfEnabled(item_info, ai_turn_speed, "ai", "turn_speed");
            saveKeyIfEnabled(item_info, ai_player_move_speed, "ai", "player_move_speed");
            saveKeyIfEnabled(item_info, ai_player_turn_speed, "ai", "player_turn_speed");
            saveKeyIfEnabled(item_info, player_collision_velocity_multiplier, "player_collision", "velocity_multiplier");
            saveKeyIfEnabled(item_info, player_collision_jump_height, "player_collision", "jump", "height");
            saveKeyIfEnabled(item_info, player_collision_jump_duration, "player_collision", "jump", "duration");
            saveKeyIfEnabled(item_info, player_collision_flip_revolutions, "player_collision", "flip", "revolutions");
            saveKeyIfEnabled(item_info, player_collision_flip_duration, "player_collision", "flip", "duration");
            saveKeyIfEnabled(item_info, player_collision_spin_revolutions, "player_collision", "spin", "revolutions");
            saveKeyIfEnabled(item_info, player_collision_spin_duration, "player_collision", "spin", "duration");
            saveKeyIfEnabled(item_info, player_collision_vertical_pos_offset, "player_collision", "vertical_pos_offset");
            saveKeyIfEnabled(item_info, grow_size_multiplier, "grow", "size_multiplier");
            saveKeyIfEnabled(item_info, grow_growth_duration, "grow", "growth_duration");
            saveKeyIfEnabled(item_info, grow_shrink_duraion, "grow", "shrink_duraion");
            saveKeyIfEnabled(item_info, grow_max_growth_duration, "grow", "max_growth_duration");
            saveKeyIfEnabled(item_info, shrink_size_multiplier, "shrink", "size_multiplier");
            saveKeyIfEnabled(item_info, shrink_growth_duration, "shrink", "growth_duration");
            saveKeyIfEnabled(item_info, shrink_shrink_duraion, "shrink", "shrink_duraion");
            saveKeyIfEnabled(item_info, shrink_max_shrink_duration, "shrink", "max_shrink_duration");
            saveKeyIfEnabled(controller_config[cam_type], pool_size, "pool_size");
            saveKeyIfEnabled(controller_config[cam_type], probabilities, "probabilities", "place_" + probabilities_position.SelectedItem.ToString());

            //Save current select for saving next time
            cam_type = itemSelect.SelectedItem.ToString();
            probabilities_position.SelectedIndex = 0;

            //Load new config
            JObject item_info_new = controller_config[cam_type]["info"].ToObject<JObject>();
            loadKeyIfEnabled(item_info_new, lifetime, "lifetime");
            loadKeyIfEnabled(item_info_new, player_immunity_time, "player_immunity_time");
            loadKeyIfEnabled(item_info_new, max_speed, "max_speed");
            loadKeyIfEnabled(item_info_new, speed, "speed");
            loadKeyIfEnabled(item_info_new, use_pos_offset, "use_pos_offset");
            loadKeyIfEnabled(item_info_new, max_bounce_count, "max_bounce_count");
            loadKeyIfEnabled(item_info_new, collision_frame_delay, "collision_frame_delay");
            loadKeyIfEnabled(item_info_new, move_toward_distance_squared, "move_toward_distance_squared");
            loadKeyIfEnabled(item_info_new, duration_multiplier, "duration_multiplier");
            loadKeyIfEnabled(item_info_new, boost_amount, "boost", "amount");
            loadKeyIfEnabled(item_info_new, boost_min_velocity, "boost", "min_velocity");
            loadKeyIfEnabled(item_info_new, boost_duration, "boost", "duration");
            loadKeyIfEnabled(item_info_new, boost_player_drag, "boost", "player_drag");
            loadKeyIfEnabled(item_info_new, boost_move_speed, "boost", "move_speed");
            loadKeyIfEnabled(item_info_new, boost_turn_speed, "boost", "turn_speed");
            loadKeyIfEnabled(item_info_new, explosion_size_multiplier, "explosion", "size_multiplier");
            loadKeyIfEnabled(item_info_new, explosion_growth_duration, "explosion", "growth_duration");
            loadKeyIfEnabled(item_info_new, throw_horizontal_pos_offset, "throw", "horizontal_pos_offset");
            loadKeyIfEnabled(item_info_new, throw_vertical_pos_offset, "throw", "vertical_pos_offset");
            loadKeyIfEnabled(item_info_new, throw_forward_force, "throw", "forward_force");
            loadKeyIfEnabled(item_info_new, throw_upward_force, "throw", "upward_force");
            loadKeyIfEnabled(item_info_new, throw_max_gravity, "throw", "max_gravity");
            loadKeyIfEnabled(item_info_new, spin_revolutions, "spin", "revolutions");
            loadKeyIfEnabled(item_info_new, spin_duration, "spin", "duration");
            loadKeyIfEnabled(item_info_new, ai_move_speed, "ai", "move_speed");
            loadKeyIfEnabled(item_info_new, ai_turn_speed, "ai", "turn_speed");
            loadKeyIfEnabled(item_info_new, ai_player_move_speed, "ai", "player_move_speed");
            loadKeyIfEnabled(item_info_new, ai_player_turn_speed, "ai", "player_turn_speed");
            loadKeyIfEnabled(item_info_new, player_collision_velocity_multiplier, "player_collision", "velocity_multiplier");
            loadKeyIfEnabled(item_info_new, player_collision_jump_height, "player_collision", "jump", "height");
            loadKeyIfEnabled(item_info_new, player_collision_jump_duration, "player_collision", "jump", "duration");
            loadKeyIfEnabled(item_info_new, player_collision_flip_revolutions, "player_collision", "flip", "revolutions");
            loadKeyIfEnabled(item_info_new, player_collision_flip_duration, "player_collision", "flip", "duration");
            loadKeyIfEnabled(item_info_new, player_collision_spin_revolutions, "player_collision", "spin", "revolutions");
            loadKeyIfEnabled(item_info_new, player_collision_spin_duration, "player_collision", "spin", "duration");
            loadKeyIfEnabled(item_info_new, player_collision_vertical_pos_offset, "player_collision", "vertical_pos_offset");
            loadKeyIfEnabled(item_info_new, grow_size_multiplier, "grow", "size_multiplier");
            loadKeyIfEnabled(item_info_new, grow_growth_duration, "grow", "growth_duration");
            loadKeyIfEnabled(item_info_new, grow_shrink_duraion, "grow", "shrink_duraion");
            loadKeyIfEnabled(item_info_new, grow_max_growth_duration, "grow", "max_growth_duration");
            loadKeyIfEnabled(item_info_new, shrink_size_multiplier, "shrink", "size_multiplier");
            loadKeyIfEnabled(item_info_new, shrink_growth_duration, "shrink", "growth_duration");
            loadKeyIfEnabled(item_info_new, shrink_shrink_duraion, "shrink", "shrink_duraion");
            loadKeyIfEnabled(item_info_new, shrink_max_shrink_duration, "shrink", "max_shrink_duration");
            loadKeyIfEnabled(controller_config[cam_type].ToObject<JObject>(), pool_size, "pool_size");
            loadKeyIfEnabled(controller_config[cam_type].ToObject<JObject>(), probabilities, "probabilities", "place_" + probabilities_position.SelectedItem.ToString());
            probability_preview.Text = probabilities.Value.ToString();
        }

        /* Select a different probability */
        private void probabilities_position_SelectedIndexChanged(object sender, EventArgs e)
        {
            controller_config[cam_type]["probabilities"]["place_" + position] = probabilities.Value;
            position = probabilities_position.SelectedItem.ToString();
            probabilities.Value = controller_config[cam_type]["probabilities"]["place_" + position].Value<int>();
            probability_preview.Text = probabilities.Value.ToString();
        }

        /* Preview current probability */
        private void probabilities_Scroll(object sender, EventArgs e)
        {
            probability_preview.Text = probabilities.Value.ToString();
        }

        /* Save the config */
        private void saveConfig_Click(object sender, EventArgs e)
        {
            //Change index to force a config update
            if (itemSelect.SelectedIndex < 12)
            {
                itemSelect.SelectedIndex = itemSelect.SelectedIndex + 1;
            }
            else
            {
                itemSelect.SelectedIndex = 0;
            }

            //Save config
            File.WriteAllText("DATA/CONFIGS/ITEM_CONFIG.JSON", controller_config.ToString(Formatting.Indented));
            MessageBox.Show("Item configurations saved.", "Saved.", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Close();
        }

        /* Handlers for saving if enabled */
        void saveKeyIfEnabled(JToken json, TextBox textbox, string key1, string key2 = "", string key3 = "")
        {
            if (!textbox.Enabled)
            {
                return;
            }

            if (key3 == "" && key2 == "")
            {
                json[key1] = textbox.Text;
            }
            else if (key3 == "")
            {
                json[key1][key2] = textbox.Text;
            }
            else
            {
                json[key1][key2][key3] = textbox.Text;
            }
        }
        void saveKeyIfEnabled(JToken json, NumericUpDown textbox, string key1, string key2 = "", string key3 = "")
        {
            if (!textbox.Enabled)
            {
                return;
            }

            if (key3 == "" && key2 == "")
            {
                json[key1] = textbox.Value;
            }
            else if (key3 == "")
            {
                json[key1][key2] = textbox.Value;
            }
            else
            {
                json[key1][key2][key3] = textbox.Value;
            }
        }
        void saveKeyIfEnabled(JToken json, TrackBar trackbar, string key1, string key2 = "", string key3 = "")
        {
            if (!trackbar.Enabled)
            {
                return;
            }

            if (key3 == "" && key2 == "")
            {
                json[key1] = trackbar.Value;
            }
            else if (key3 == "")
            {
                json[key1][key2] = trackbar.Value;
            }
            else
            {
                json[key1][key2][key3] = trackbar.Value;
            }
        }

        /* Handlers for setting if not null */
        void loadKeyIfEnabled(JObject json, TextBox textbox, string key1, string key2 = "", string key3 = "") //must be string!
        {
            textbox.Text = "";
            textbox.Enabled = false;
            if (key3 == "" && key2 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }

                textbox.Text = json[key1].Value<string>();
            }
            else if (key3 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub = json[key1].ToObject<JObject>();
                if (!sub.ContainsKey(key2))
                {
                    return;
                }

                textbox.Text = json[key1][key2].Value<string>();
            }
            else
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub1 = json[key1].ToObject<JObject>();
                if (!sub1.ContainsKey(key2))
                {
                    return;
                }
                JObject sub = json[key1][key2].ToObject<JObject>();
                if (!sub.ContainsKey(key3))
                {
                    return;
                }

                textbox.Text = json[key1][key2][key3].Value<string>();
            }
            textbox.Enabled = true;
        }
        void loadKeyIfEnabled(JObject json, NumericUpDown textbox, string key1, string key2 = "", string key3 = "") //must be decimal!
        {
            textbox.Value = 0;
            textbox.Enabled = false;
            if (key3 == "" && key2 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }

                textbox.Value = json[key1].Value<decimal>();
            }
            else if (key3 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub = json[key1].ToObject<JObject>();
                if (!sub.ContainsKey(key2))
                {
                    return;
                }

                textbox.Value = json[key1][key2].Value<decimal>();
            }
            else
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub1 = json[key1].ToObject<JObject>();
                if (!sub1.ContainsKey(key2))
                {
                    return;
                }
                JObject sub = json[key1][key2].ToObject<JObject>();
                if (!sub.ContainsKey(key3))
                {
                    return;
                }

                textbox.Value = json[key1][key2][key3].Value<decimal>();
            }
            textbox.Enabled = true;
        }
        void loadKeyIfEnabled(JObject json, TrackBar trackbar, string key1, string key2 = "", string key3 = "") //must be int!
        {
            trackbar.Value = 0;
            trackbar.Enabled = false;
            if (key3 == "" && key2 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }

                trackbar.Value = json[key1].Value<int>();
            }
            else if (key3 == "")
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub = json[key1].ToObject<JObject>();
                if (!sub.ContainsKey(key2))
                {
                    return;
                }

                trackbar.Value = json[key1][key2].Value<int>();
            }
            else
            {
                if (!json.ContainsKey(key1))
                {
                    return;
                }
                JObject sub1 = json[key1].ToObject<JObject>();
                if (!sub1.ContainsKey(key2))
                {
                    return;
                }
                JObject sub = json[key1][key2].ToObject<JObject>();
                if (!sub.ContainsKey(key3))
                {
                    return;
                }

                trackbar.Value = json[key1][key2][key3].Value<int>();
            }
            trackbar.Enabled = true;
        }






        private void loadItem_Click(object sender, EventArgs e)
        {

        }
    }
}
