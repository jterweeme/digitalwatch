/*
2014 Jasper ter Weeme
*/

module vga(input clk, output r, output g, output b, output hsync, output vsync);
    wire clk25;

    klokdeler klok(.clk_in(clk), .clk_out(clk25));
    display scherm(.clk(clk25), .hsync(hsync), .vsync(vsync), .red(r), .green(g), .blue(b));
endmodule



