-- ========================================================================
--                           Projekt do pøedmìtu HSC
--                           -----------------------
--
--   Jméno a Pøíjmení: Petr Zemek
--
--   Login: xzemek02
--   Datum: 28.12.2008
--
-- ========================================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- ------------------------ Entity declaration --------------------------------
entity USR_PROC is
   port (
      -- Øídicí signály
      CLK                : in  std_logic;
      RESET              : in  std_logic;
      -- Rozhraní procesoru PicoBlaze
      CPU_PORT_ID        : in  std_logic_vector(7 downto 0);
      CPU_WRITE_STROBE   : in  std_logic;
      CPU_READ_STROBE    : in  std_logic;
      CPU_OUT_PORT       : in  std_logic_vector(7 downto 0);
      CPU_IN_PORT        : out std_logic_vector(7 downto 0);
      -- Rozhraní 9-okolí
      R0,R1,R2           : in  std_logic_vector(7 downto 0);
      R3,R4,R5           : in  std_logic_vector(7 downto 0);
      R6,R7,R8           : in  std_logic_vector(7 downto 0);
      -- Øízení vstupu dat
      INPUT_FIFO_FULL    : in  std_logic;
      INPUT_FIFO_EMPTY   : in  std_logic;
      INPUT_FIFO_DATA    : in  std_logic_vector(7 downto 0);
      NEXT_PIXEL         : out std_logic;
      -- Výstupní rozhraní
      OUT_DATA           : out std_logic_vector(7 downto 0);
      OUT_DATA_VLD       : out std_logic
      );
end USR_PROC;

-- ------------------------- Architecture declaration -------------------------

architecture behav of USR_PROC is

   -- Mapování registrù do adresového prostoru procesoru PicoBlaze
   constant PORT_REG_R0       : std_logic_vector(7 downto 0):=X"00";
   constant PORT_REG_R1       : std_logic_vector(7 downto 0):=X"01";
   constant PORT_REG_R2       : std_logic_vector(7 downto 0):=X"02";
   constant PORT_REG_R3       : std_logic_vector(7 downto 0):=X"03";
   constant PORT_REG_R4       : std_logic_vector(7 downto 0):=X"04";
   constant PORT_REG_R5       : std_logic_vector(7 downto 0):=X"05";
   constant PORT_REG_R6       : std_logic_vector(7 downto 0):=X"06";
   constant PORT_REG_R7       : std_logic_vector(7 downto 0):=X"07";
   constant PORT_REG_R8       : std_logic_vector(7 downto 0):=X"08";
   constant PORT_REG_Rout     : std_logic_vector(7 downto 0):=X"20";
   constant PORT_REG_MEDIAN   : std_logic_vector(7 downto 0):=X"21";
   constant PORT_REG_STATUS   : std_logic_vector(7 downto 0):=X"FE";
   constant PORT_REG_CONTROL  : std_logic_vector(7 downto 0):=X"FF";

   -- Øídicí registr
   signal reg_control      : std_logic_vector(7 downto 0);
   signal reg_control_we   : std_logic;

   -- Status registr
   signal reg_status       : std_logic_vector(7 downto 0);

   -- Výstupní registr
   signal reg_Rout         : std_logic_vector(7 downto 0);
   signal reg_Rout_vld     : std_logic;
   signal reg_Rout_we      : std_logic;

   -- 9-okolí bylo právì posunuto v obraze o jeden pixel
   signal pixels_shifted   : std_logic;

   -- Median
   signal median_value     : std_logic_vector(7 downto 0);

    -- Serazovaci sit
    signal A0 : std_logic_vector(7 downto 0);
    signal A1 : std_logic_vector(7 downto 0);
    signal A2 : std_logic_vector(7 downto 0);
    signal A3 : std_logic_vector(7 downto 0);
    signal A5 : std_logic_vector(7 downto 0);
    signal A6 : std_logic_vector(7 downto 0);
    signal A7 : std_logic_vector(7 downto 0);
    signal A8 : std_logic_vector(7 downto 0);
    signal B0 : std_logic_vector(7 downto 0);
    signal B1 : std_logic_vector(7 downto 0);
    signal B3 : std_logic_vector(7 downto 0);
    signal B4 : std_logic_vector(7 downto 0);
    signal B5 : std_logic_vector(7 downto 0);
    signal B7 : std_logic_vector(7 downto 0);
    signal C0 : std_logic_vector(7 downto 0);
    signal C2 : std_logic_vector(7 downto 0);
    signal C3 : std_logic_vector(7 downto 0);
    signal C4 : std_logic_vector(7 downto 0);
    signal C5 : std_logic_vector(7 downto 0);
    signal C8 : std_logic_vector(7 downto 0);
    signal D0 : std_logic_vector(7 downto 0);
    signal D1 : std_logic_vector(7 downto 0);
    signal D2 : std_logic_vector(7 downto 0);
    signal D4 : std_logic_vector(7 downto 0);
    signal D6 : std_logic_vector(7 downto 0);
    signal D8 : std_logic_vector(7 downto 0);
    signal E2 : std_logic_vector(7 downto 0);
    signal E4 : std_logic_vector(7 downto 0);
    signal E6 : std_logic_vector(7 downto 0);
    signal E8 : std_logic_vector(7 downto 0);
    signal F1 : std_logic_vector(7 downto 0);
    signal F3 : std_logic_vector(7 downto 0);
    signal F4 : std_logic_vector(7 downto 0);
    signal F6 : std_logic_vector(7 downto 0);
    signal G3 : std_logic_vector(7 downto 0);
    signal G4 : std_logic_vector(7 downto 0);

	signal reg_A0 : std_logic_vector(7 downto 0);
    signal reg_A1 : std_logic_vector(7 downto 0);
    signal reg_A2 : std_logic_vector(7 downto 0);
    signal reg_A3 : std_logic_vector(7 downto 0);
    signal reg_A5 : std_logic_vector(7 downto 0);
    signal reg_A6 : std_logic_vector(7 downto 0);
    signal reg_A7 : std_logic_vector(7 downto 0);
    signal reg_A8 : std_logic_vector(7 downto 0);
	signal reg_B0 : std_logic_vector(7 downto 0);
    signal reg_B1 : std_logic_vector(7 downto 0);
    signal reg_B3 : std_logic_vector(7 downto 0);
    signal reg_B4 : std_logic_vector(7 downto 0);
    signal reg_B5 : std_logic_vector(7 downto 0);
    signal reg_B7 : std_logic_vector(7 downto 0);
    signal reg_C0 : std_logic_vector(7 downto 0);
    signal reg_C2 : std_logic_vector(7 downto 0);
    signal reg_C3 : std_logic_vector(7 downto 0);
    signal reg_C4 : std_logic_vector(7 downto 0);
    signal reg_C5 : std_logic_vector(7 downto 0);
    signal reg_C8 : std_logic_vector(7 downto 0);
    signal reg_D0 : std_logic_vector(7 downto 0);
    signal reg_D1 : std_logic_vector(7 downto 0);
    signal reg_D2 : std_logic_vector(7 downto 0);
    signal reg_D4 : std_logic_vector(7 downto 0);
    signal reg_D6 : std_logic_vector(7 downto 0);
    signal reg_D8 : std_logic_vector(7 downto 0);
    signal reg_E2 : std_logic_vector(7 downto 0);
    signal reg_E4 : std_logic_vector(7 downto 0);
    signal reg_E6 : std_logic_vector(7 downto 0);
    signal reg_E8 : std_logic_vector(7 downto 0);
    signal reg_F1 : std_logic_vector(7 downto 0);
    signal reg_F3 : std_logic_vector(7 downto 0);
    signal reg_F4 : std_logic_vector(7 downto 0);
    signal reg_F6 : std_logic_vector(7 downto 0);
    signal reg_G3 : std_logic_vector(7 downto 0);
    signal reg_G4 : std_logic_vector(7 downto 0);

begin

   -- ------------------------ HW akcelerace ---------------------------------

   -- ------------------- Serazovani pomoci radici site ----------------------

    -- [[0,8], [1,5], [2,6], [3,7]]
    sort1 : process(R0, R8, R1, R5, R2, R6, R3, R7)
    begin
        -- [0,8]
        if R0 < R8 then
            A0 <= R0;
            A8 <= R8;
        else
            A0 <= R8;
            A8 <= R0;
        end if;

        -- [1,5]
        if R1 < R5 then
            A1 <= R1;
            A5 <= R5;
        else
            A1 <= R5;
            A5 <= R1;
        end if;

        -- [2,6]
        if R2 < R6 then
            A2 <= R2;
            A6 <= R6;
        else
            A2 <= R6;
            A6 <= R2;
        end if;

        -- [3,7]
        if R3 < R7 then
            A3 <= R3;
            A7 <= R7;
        else
            A3 <= R7;
            A7 <= R3;
        end if;
    end process sort1;

    -- [[0,4], [1,3], [5,7]]
    sort2 : process(A0, R4, A1, A3, A5, A7)
    begin
        -- [0,4]
        if A0 < R4 then
            B0 <= A0;
            B4 <= R4;
        else
            B0 <= R4;
            B4 <= A0;
        end if;

        -- [1,3]
        if A1 < A3 then
            B1 <= A1;
            B3 <= A3;
        else
            B1 <= A3;
            B3 <= A1;
        end if;

        -- [5,7]
        if A5 < A7 then
            B5 <= A5;
            B7 <= A7;
        else
            B5 <= A7;
            B7 <= A5;
        end if;
    end process sort2;

    -- [[4,8], [0,2], [3,5]]
    sort3 : process(B4, A8, B0, A2, B3, B5)
    begin
        -- [4,8]
        if B4 < A8 then
            C4 <= B4;
            C8 <= A8;
        else
            C4 <= A8;
            C8 <= B4;
        end if;

        -- [0,2]
        if B0 < A2 then
            C0 <= B0;
            C2 <= A2;
        else
            C0 <= A2;
            C2 <= B0;
        end if;

        -- [3,5]
        if B3 < B5 then
            C3 <= B3;
            C5 <= B5;
        else
            C3 <= B5;
            C5 <= B3;
        end if;
    end process sort3;

    -- Naplneni registru
    process(CLK)
    begin
        if ((CLK'event) and (CLK = '1')) then
            reg_C4 <= C4;
            reg_A6 <= A6;
            reg_C2 <= C2;
            reg_C8 <= C8;
            reg_C0 <= C0;
            reg_B1 <= B1;
        end if;
    end process;

    -- [[4,6], [2,8], [0,1]]
    sort4 : process(reg_C4, reg_A6, reg_C2, reg_C8, reg_C0, reg_B1)
    begin
        -- [4,6]
        if reg_C4 < reg_A6 then
            D4 <= reg_C4;
            D6 <= reg_A6;
        else
            D4 <= reg_A6;
            D6 <= reg_C4;
        end if;

        -- [2,8]
        if reg_C2 < reg_C8 then
            D2 <= reg_C2;
            D8 <= reg_C8;
        else
            D2 <= reg_C8;
            D8 <= reg_C2;
        end if;

        -- [0,1]
        if reg_C0 < reg_B1 then
            D0 <= reg_C0;
            D1 <= reg_B1;
        else
            D0 <= reg_B1;
            D1 <= reg_C0;
        end if;
    end process sort4;

    -- [[2,4], [6,8]]
    sort5 : process(D2, D4, D6, D8)
    begin
        -- [2,4]
        if D2 < D4 then
            E2 <= D2;
            E4 <= D4;
        else
            E2 <= D4;
            E4 <= D2;
        end if;

        -- [6,8]
        if D6 < D8 then
            E6 <= D6;
            E8 <= D8;
        else
            E6 <= D8;
            E8 <= D6;
        end if;
    end process sort5;

    -- Naplneni registru
    process(CLK)
    begin
        if ((CLK'event) and (CLK = '1')) then
            reg_E2 <= E2;
            reg_C3 <= C3;
            reg_E4 <= E4;
            reg_C5 <= C5;
            reg_E6 <= E6;
            reg_B7 <= B7;
            reg_D1 <= D1;
            reg_E8 <= E8;
        end if;
    end process;

    -- [[2,3], [4,5], [6,7], [1,8]]
    sort6 : process(reg_E2, reg_C3, reg_E4, reg_C5, reg_E6, reg_B7, reg_D1, reg_E8)
    begin
        -- [2,3]
        if reg_E2 < reg_C3 then
            F3 <= reg_C3;
        else
            F3 <= reg_E2;
        end if;

        -- [4,5]
        if reg_E4 < reg_C5 then
            F4 <= reg_E4;
        else
            F4 <= reg_C5;
        end if;

        -- [6,7]
        if reg_E6 < reg_B7 then
            F6 <= reg_E6;
        else
            F6 <= reg_B7;
        end if;

        -- [1,8]
        if reg_D1 < reg_E8 then
            F1 <= reg_D1;
        else
            F1 <= reg_E8;
        end if;
    end process sort6;

    -- [[1,4], [3,6]]
    sort7 : process(F1, F3, F4, F6)
    begin
        if F1 < F4 then
            if F3 < F6 then
                if F3 < F4 then
                    median_value <= F4;
                else
                    median_value <= F3;
                end if;
            else
                if F6 < F4 then
                    median_value <= F4;
                else
                    median_value <= F6;
                end if;
            end if;
        else
            if F3 < F6 then
                if F3 < F1 then
                    median_value <= F1;
                else
                    median_value <= F3;
                end if;
            else
                if F6 < F1 then
                    median_value <= F1;
                else
                    median_value <= F6;
                end if;
            end if;
        end if;
    end process sort7;

   -- ---------------- Posun 9-okolí v obraze o 1 pixel ---------------------

   -- Signalizace posunu 9-okolí v obrazu o jeden pixel
   NEXT_PIXEL <= reg_control_we and CPU_OUT_PORT(0);

   -- 9-okolí bylo právì posunuto v obraze o jeden pixel
   -- (zpo¾dìní NEXT_PIXEL signálu o 1 takt CLK)
   shift_pixels_proc :  process (CLK)
   begin
      if CLK'event and CLK = '1' then
         pixels_shifted <= reg_control_we and CPU_OUT_PORT(0);
      end if;
   end process shift_pixels_proc;

   -- ----------------------- Øídicí registr --------------------------------
   control_register :  process (CLK, RESET)
   begin
      if RESET = '1' then
         reg_control <= (others => '0');
      elsif CLK'event and CLK = '1' then
         if reg_control_we = '1' then
            reg_control <= CPU_OUT_PORT;
         end if;
      end if;
   end process control_register;

   -- ----------------------- Stavový registr -------------------------------
   reg_status <= (7 downto 2 => '0') & INPUT_FIFO_FULL &
                 (not INPUT_FIFO_EMPTY);

   -- ----------------------- Výstupní registr ------------------------------
   output_register :  process (CLK, RESET)
   begin
      if RESET = '1' then
         reg_Rout     <= (others => '0');
      elsif CLK'event and CLK = '1' then
         if reg_Rout_we = '1' then
            reg_Rout     <= CPU_OUT_PORT;
         end if;
      end if;
   end process output_register;

   -- Výstupní data jsou nejprve ulo¾ena do registru reg_Rout a a¾ pak
   -- (po 1 hodinovém cyklu) odeslány na výstup. Potvrzení dat signálem
   -- OUT_DATA_VLD je tedy nutné zpozdit také o 1 hodinový cyklus.
   output_valid :  process (CLK, RESET)
   begin
      if CLK'event and CLK = '1' then
         reg_Rout_vld <= reg_Rout_we;
      end if;
   end process output_valid;

   -- Pøiøazení dat na výstup
   OUT_DATA     <= reg_Rout;
   OUT_DATA_VLD <= reg_Rout_vld;

   -- ------------------------ Adresový dekodér ------------------------------
   --
   --  Port   Pøístup         Popis
   -- --------------------------------------------------------------------
   --
   --  0x20    Read, Write    Registr Rout pro vysílání výsledkù
   --
   --  0x21 ...               Volné porty - je mo¾né vlo¾it vlastní
   --                         registry pro komunikaci mezi procesorem
   --                         akcelerovanou èástí
   --  0x21    Read           Registr REG_MEDIAN s vypoctenym medianem
   --
   --  0xFE    Read           Registr má na 0-tém bitu indikaci stavu vstupní
   --                         fronty. Pokud je bit nastaven, jsou ve FIFO
   --                         pamìti data z kamery.
   --
   --  0xFF    Read/Write     Zápisem jednièky na 0-tý bit se posune
   --                         9-okolí o jeden pixel.
   --

   -- -------------- Zápisy do registrù ------------
   -- Zápis do registru Rout
   reg_Rout_we   <= '1' when (CPU_PORT_ID=PORT_REG_Rout    and
                              CPU_WRITE_STROBE='1') else
                    '0';

   -- Zápis do registru Control
   reg_control_we<= '1' when (CPU_PORT_ID=PORT_REG_CONTROL and
                              CPU_WRITE_STROBE='1') else
                    '0';

   -- -------------- Ètení z registrù --------------
   adec_read : process  (CPU_PORT_ID, reg_Rout, reg_status, reg_control,
                         median_value)
   begin
      CPU_IN_PORT <= (others => '0');
      case CPU_PORT_ID is

         -- Registr s vypoctenym medianem
         when PORT_REG_MEDIAN =>  CPU_IN_PORT <= median_value;

         -- Výstupní registr
         when PORT_REG_Rout =>  CPU_IN_PORT <= reg_Rout;

         -- Øídicí a status registr
         when PORT_REG_STATUS  =>  CPU_IN_PORT <= reg_status;
         when PORT_REG_CONTROL =>  CPU_IN_PORT <= reg_control;

         when others=> null;
      end case;
   end process adec_read;

end behav;
