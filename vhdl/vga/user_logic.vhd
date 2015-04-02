------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Wed Nov 23 11:06:30 2011 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Addr                  -- Bus to IP address bus
--   Bus2IP_CS                    -- Bus to IP chip select
--   Bus2IP_RNW                   -- Bus to IP read/not write
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 2
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
	 VGA_RED : out  STD_LOGIC;
    VGA_GREEN : out  STD_LOGIC;
    VGA_BLUE : out  STD_LOGIC;
    VGA_HSYNC : out  STD_LOGIC;
    VGA_VSYNC : out  STD_LOGIC;
	 
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Addr                    : in  std_logic_vector(0 to 31);
    Bus2IP_CS                      : in  std_logic_vector(0 to 0);
    Bus2IP_RNW                     : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic
  signal data_out :STD_LOGIC_VECTOR (8 downto 0);
  signal addr_out :STD_LOGIC_VECTOR (12 downto 0);
  signal wea : STD_LOGIC;
  signal temp: STD_LOGIC_VECTOR (8 downto 0);
  type state_type is (st0_null,st1_read, st2_gen, st3_write); 
  signal state, next_state : state_type :=st0_null; 
  
  	signal addrb_wire 	:STD_LOGIC_VECTOR (12 DOWNTO 0);
	signal doutb_wire	   :STD_LOGIC_VECTOR (8 DOWNTO  0);
	signal out_25mhz_wire:STD_LOGIC;
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_reg1                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_reg_write_sel              : std_logic_vector(0 to 1);
  signal slv_reg_read_sel               : std_logic_vector(0 to 1);
  signal slv_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

	COMPONENT vga_ram
	  PORT (
		 clka : IN STD_LOGIC;
		 wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		 addra : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
		 dina : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
		 douta : OUT STD_LOGIC_VECTOR(8 DOWNTO 0);
		 clkb : IN STD_LOGIC;
		 web : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		 addrb : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
		 dinb : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
		 doutb : OUT STD_LOGIC_VECTOR(8 DOWNTO 0)
	  );
	 END COMPONENT;

	COMPONENT vga_sync
		PORT(
			clk : IN std_logic;
			reset : IN std_logic;
			ram_d : IN std_logic_vector(8 downto 0);          
			ram_addr : OUT std_logic_vector(12 downto 0);
			red : OUT std_logic;
			green : OUT std_logic;
			blue : OUT std_logic;
			hsync : OUT std_logic;
			vsync : OUT std_logic
		);
	END COMPONENT;

	COMPONENT clock
	PORT(
		In_50m : IN std_logic;          
		Out_25m : OUT std_logic
		);
	END COMPONENT;

begin
	Inst_vga_ram : vga_ram
	  PORT MAP (
		 clka => Bus2IP_Clk,
		 wea(0) => wea,
		 addra => addr_out,
		 dina => data_out,
		 douta => temp,
		 clkb => Bus2IP_Clk,
		 web => "0",
		 addrb => addrb_wire,
		 dinb => "XXXXXXXXX",
		 doutb => doutb_wire
	  );

	Inst_vga_sync: vga_sync PORT MAP(
		clk => out_25mhz_wire,
		reset => '1',
		ram_d => doutb_wire,
		ram_addr => addrb_wire,
		red => VGA_RED,
		green => VGA_GREEN,
		blue => VGA_BLUE,
		hsync => VGA_HSYNC,
		vsync => VGA_VSYNC
	);	
	
	Inst_clock: clock PORT MAP(
		In_50m => Bus2IP_Clk,
		Out_25m => out_25mhz_wire
	);
	
  --USER logic implementation added here
   SYNC_PROC: process (Bus2IP_Clk)
   begin
      if (Bus2IP_Clk'event and Bus2IP_Clk = '1') then
         if (slv_reg1(31)= '1') then
            state <= st1_read;
         else
            state <= next_state;
         -- assign other outputs to internal signals
         end if;        
      end if;
   end process;
	
   OUTPUT_DECODE: process (state)
   begin
      --insert statements to decode internal output signals
      --below is simple example
      if (state = st1_read) then
			data_out <= "XXXXXXXXX";
			slv_reg1(30) <= '0';
      elsif (state = st2_gen) then
			slv_reg1(30) <= '0';
			case (slv_reg0(27 to 28)) is
				when "00" =>
					data_out <= temp(8 downto 3)&slv_reg0(29 to 31);
				when "01" =>
					data_out <= temp(8 downto 6)&slv_reg0(29 to 31)&temp(2 downto 0);
				when "10" =>
					data_out <= slv_reg0(29 to 31)&temp(5 downto 0);
				when others => data_out <= "111111111";
			end case;
		elsif (state = st3_write) then
			case (slv_reg0(27 to 28)) is
				when "00" =>
					data_out <= temp(8 downto 3)&slv_reg0(29 to 31);
				when "01" =>
					data_out <= temp(8 downto 6)&slv_reg0(29 to 31)&temp(2 downto 0);
				when "10" =>
					data_out <= slv_reg0(29 to 31)&temp(5 downto 0);
				when others => data_out <= "111111111";
			end case;
			slv_reg1(30) <= '0';
			wea<='1';
		else
			slv_reg1(30) <= '1';
			wea<='0';
			case (slv_reg0(27 to 28)) is
				when "00" =>
					data_out <= temp(8 downto 3)&slv_reg0(29 to 31);
				when "01" =>
					data_out <= temp(8 downto 6)&slv_reg0(29 to 31)&temp(2 downto 0);
				when "10" =>
					data_out <= slv_reg0(29 to 31)&temp(5 downto 0);
				when others => data_out <= "111111111";
			end case;
      end if;
   end process;
	
   NEXT_STATE_DECODE: process (state)
   begin
      --declare default state for next_state to avoid latches
      next_state <= state;  --default is to stay in current state
      --insert statements to decode next_state
      --below is a simple example
      case (state) is
         when st1_read =>
            next_state <= st2_gen;
         when st2_gen =>
            next_state <= st3_write;
         when st3_write =>
            next_state <= st0_null;
         when others =>
            next_state <= st0_null;
      end case;      
   end process;
  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(0 to 1);
  slv_reg_read_sel  <= Bus2IP_RdCE(0 to 1);
  slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1);
  slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1);

  -- implement slave model software accessible register(s)
  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk, state ) is
  begin

    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        slv_reg0 <= (others => '0');
        slv_reg1(31) <= '0';
      elsif (slv_reg_write_sel="10") then
			if (state = st1_read) then
				slv_reg1(31)<='0';
			end if;
			for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
			  if ( Bus2IP_BE(byte_index) = '1' ) then
				 slv_reg0(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
			  end if;
			end loop;
		else
			if (state = st1_read) then
				slv_reg1(31)<='0';
			elsif (slv_reg_write_sel="01") then
				slv_reg1(31)<=Bus2IP_Data(31);
			end if;
      end if;
    end if;
--    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
--      if Bus2IP_Reset = '1' then
--        slv_reg0 <= (others => '0');
--        slv_reg1 <= (others => '0');
--      else
--        case slv_reg_write_sel is
--          when "10" =>
--            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
--              if ( Bus2IP_BE(byte_index) = '1' ) then
--                slv_reg0(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
--              end if;
--            end loop;
--          when "01" =>
--            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
--              if ( Bus2IP_BE(byte_index) = '1' ) then
--                slv_reg1(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
--              end if;
--            end loop;
--          when others => null;
--        end case;
--      end if;
--    end if;

  end process SLAVE_REG_WRITE_PROC;
  
  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0, slv_reg1 ) is
  begin

    case slv_reg_read_sel is
      when "10" => slv_ip2bus_data <= slv_reg0;
      when "01" => slv_ip2bus_data <= slv_reg1;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;
	
  addr_out <= slv_reg0(14 to 26);
  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';

end IMP;
