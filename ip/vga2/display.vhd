-- 2014 Jasper ter Weeme

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity display is
    port (clk: in std_logic; hsync, vsync, red, green, blue: out std_logic);
end display;

architecture behavior of display is
    signal h_sync, v_sync, video_en, horizontal_en, vertical_en, red_signal, green_signal, blue_signal: std_logic;
    signal h_cnt, v_cnt: unsigned(9 downto 0);
begin
    video_en <= horizontal_en and vertical_en;
    red <= red_signal and video_en;
    green <= green_signal and video_en;
    blue <= blue_signal and video_en;
    hsync <= h_sync;
    vsync <= v_sync;

    process begin
        wait until rising_edge(clk);

        if h_cnt = 799 then
            h_cnt <= (others => '0');
        else
            h_cnt <= h_cnt + 1;
        end if;

        if v_cnt >= 0 and v_cnt <= 59 then
            red_signal <= '1';
            green_signal <= '1';
            blue_signal <= '1';
        end if;

        if v_cnt >= 60 and v_cnt <= 119 then
            red_signal <= '0';
            green_signal <= '0';
            blue_signal <= '0';
        end if;

        if v_cnt >= 120 and v_cnt <= 179 then
            red_signal <= '1';
            green_signal <= '0';
            blue_signal <= '0';
        end if;

        if v_cnt >= 180 and v_cnt <= 239 then
            red_signal <= '0';
            green_signal <= '1';
            blue_signal <= '0';
        end if;

        if v_cnt >= 240 and v_cnt <= 299 then
            red_signal <= '0';
            green_signal <= '0';
            blue_signal <= '1';
        end if;

        if v_cnt >= 300 and v_cnt <= 359 then
            red_signal <= '1';
            green_signal <= '1';
            blue_signal <= '0';
        end if;

        if v_cnt >= 360 and v_cnt <= 419 then
            red_signal <= '1';
            green_signal <= '0';
            blue_signal <= '1';
        end if;

        if v_cnt >= 420 and v_cnt <= 479 then
            red_signal <= '0';
            green_signal <= '1';
            blue_signal <= '1';
        end if;

        if h_cnt <= 755 and h_cnt >= 659 then
            h_sync <= '0';
        else
            h_sync <= '1';
        end if;

        if v_cnt >= 524 and h_cnt >= 699 then
            v_cnt <= (others => '0');
        elsif h_cnt = 699 then
            v_cnt <= v_cnt + 1;
        end if;

        if v_cnt <= 494 and v_cnt >= 493 then
            v_sync <= '0';
        else
            v_sync <= '1';
        end if;

        if h_cnt <= 639 then
            horizontal_en <= '1';
        else
            horizontal_en <= '0';
        end if;

        if v_cnt <= 479 then
            vertical_en <= '1';
        else
            vertical_en <= '0';
        end if;
    end process;
end behavior;


