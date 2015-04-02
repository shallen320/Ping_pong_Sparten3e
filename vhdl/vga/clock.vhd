----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:07:07 11/09/2011 
-- Design Name: 
-- Module Name:    clock - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity clock is
    Port ( In_50m : in  STD_LOGIC;
           Out_25m : out  STD_LOGIC);
end clock;

architecture Behavioral of clock is
signal count: STD_LOGIC :='0';
begin
	process (In_50m)
	begin
		if (In_50m'event and In_50m='1') then
			count <= NOT count;
		end if;
	end process;
	
	Out_25m <= count;
end Behavioral;

