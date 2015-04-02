----------------------------------------------------------------------------------
-- Company: UCSB ECE
-- Engineer: Yuepei Hu
-- 
-- Create Date:    14:10:02 11/11/2011 
-- Design Name: 
-- Module Name:    vga_sync - Behavioral 
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
use IEEE.std_logic_arith.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity vga_sync is
    Port ( clk : in  STD_LOGIC;
			  reset: in STD_LOGIC;
			  ram_d: in STD_LOGIC_VECTOR (8 downto 0);
			  ram_addr: out STD_LOGIC_VECTOR (12 DOWNTO 0);
           red : out  STD_LOGIC;
           green : out  STD_LOGIC;
           blue : out  STD_LOGIC;
           hsync : out  STD_LOGIC;
           vsync : out  STD_LOGIC);
end vga_sync;

architecture Behavioral of vga_sync is

   signal h_count: INTEGER:= 0;
	signal v_count: INTEGER:= 0;
	signal addr: INTEGER RANGE 0 TO 8191:=0;
	signal pix: INTEGER:=0;
	signal remain : INTEGER RANGE 0 to 2 :=0;
					
begin	

	process (clk)
	begin  
		if (clk'event and clk = '1') then
			if reset = '0' then
				h_count <= 0;
				v_count <= 0;
			elsif (h_count<799) then
				h_count <= h_count+1;
			else
				h_count<=0;
				v_count<= v_count+1;
				if (v_count=520) then
					v_count<=0;
				end if;
			end if;
		end if;
	end process;
				
	process (h_count)
	begin
		if (h_count < 96) then
			hsync <= '0';
		elsif (h_count < 144) then
			hsync <= '1';
		elsif (h_count < 784) then
			hsync <= '1';
		else
			hsync <= '1';
		end if;
	end process;
	
	process (v_count)
	begin
		if (v_count < 2) then
			vsync <= '0';
		elsif (v_count < 31) then
			vsync <= '1';
		elsif (v_count < 511) then
			vsync <= '1';
		else
			vsync <= '1';
		end if;
	end process;
	
	process (clk)
	begin
		if(clk'event and clk='0') then
			if (v_count<=30) then
				pix <=0;
				addr<=0;
				remain<=0;
			elsif ((v_count > 30) and (v_count < 223) and (h_count > 143) and (h_count < 272))then
				pix <= pix+1;
				if (remain=2) then
					remain <= 0;
				else
					remain <= remain+1;
				end if;
				if (remain=2) then
					if (addr=8191) then
						addr<=0;
					else
						addr<= addr+1;
					end if;
				end if;
			elsif (v_count>=511) then
				remain<=0;
				addr <= 0;
				pix <= 0;
			end if;
		end if;
	end process;
	
	process (v_count, h_count,ram_d)
	begin
		if ((v_count > 30) and (v_count < 223) and (h_count > 143) and (h_count < 272))then
			case (remain) is 
				when 1 =>
					red <= ram_d(2);
					green<=ram_d(1);
					blue <=ram_d(0);
				when 2 =>
					red <= ram_d(5);
					green<=ram_d(4);
					blue <=ram_d(3);
				when 0 =>
					red <= ram_d(8);
					green<=ram_d(7);
					blue <=ram_d(6);
				when others =>
					red <='0';
					green<='0';
					blue <='0';
			end case;
		else
			red <='0';
			green<='0';
			blue <='0';
		end if;
	end process;
	
	ram_addr <= conv_std_logic_vector(addr, 13);
end Behavioral;

