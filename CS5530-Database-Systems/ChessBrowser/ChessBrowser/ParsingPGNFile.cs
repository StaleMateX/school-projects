using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;
using Microsoft.Maui.Storage;

namespace ChessBrowser
{
    /// <summary>
    /// Class representing a single chess game
    /// </summary>
    public class ChessGame
    {
        /// <summary>
        /// event name
        /// </summary>
        public string? Event { get; set; }
        /// <summary>
        /// event site
        /// </summary>
        public string? Site { get; set; }
        /// <summary>
        /// the round in which the game took place
        /// </summary>
        public string? Round { get; set; }
        /// <summary>
        /// the name of the white player
        /// </summary>
        public string? White { get; set; }
        /// <summary>
        /// the name of the black player
        /// </summary>
        public string? Black { get; set; }
        /// <summary>
        /// the white player's Elo rating at the time of the game
        /// </summary>
        public int WhiteElo { get; set; }
        /// <summary>
        /// the black player's Elo rating at the time of the game
        /// </summary>
        public int BlackElo { get; set; }
        /// <summary>
        /// the result of the game.
        /// </summary>
        public char Result { get; set; }
        /// <summary>
        /// the event date 
        /// </summary>
        public string? EventDate { get; set; }
        /// <summary>
        /// the date 
        /// </summary>
        public string? Date { get; set; }
        /// <summary>
        /// The text representing the moves. This always comes after a blank line following the above tagged data.
        /// </summary>
        public string Moves { get; set; }
        /// <summary>
        /// moves list
        /// </summary>
        public ChessGame()
        {
            Moves = "";
        }
    }
    /// <summary>
    /// Static class that be able to read PGN files
    /// </summary>
    public static class ParsingPGNFile
    {
        /// <summary>
        /// Method to read games from a PGN file
        /// </summary>
        /// <param name="filename">the file name</param>
        /// <returns>the list of the game detail</returns>
        public static List<ChessGame> LoadPGN(string fileName)
        {

            List<ChessGame> games = new List<ChessGame>();
            string[] fileContent = File.ReadAllLines(fileName);
            ChessGame game = new ChessGame();
            StringBuilder moves = new StringBuilder();

            foreach (string line in fileContent)
            {
                if (line == "")
                {
                    if (moves.Length > 0)
                    {
                        game.Moves = moves.ToString();
                        games.Add(game);

                        game = new ChessGame();
                        moves.Clear();
                    }
                    continue;
                }


                if (line.StartsWith("[Event "))
                {
                    // dirty website
                    if (GetValue(line).Contains('?'))
                    {
                        game.Event = "?";
                        continue;
                    }
                    game.Event = GetValue(line);
                }
                else if (line.StartsWith("[Site "))
                {
                    game.Site = GetValue(line);
                }
                else if (line.StartsWith("[Date "))
                {
                    if (GetValue(line).Contains('?'))
                    {
                        game.Date = "000-00-00";
                        continue;
                    }
                    continue;
                }
                else if (line.StartsWith("[Round "))
                {
                    game.Round = GetValue(line);
                }
                else if (line.StartsWith("[White "))
                {
                    game.White = GetValue(line);
                }
                else if (line.StartsWith("[Black "))
                {
                    game.Black = GetValue(line);
                }
                else if (line.StartsWith("[WhiteElo "))
                {
                    int.TryParse(GetValue(line), out int whiteElo);
                    game.WhiteElo = whiteElo;
                }
                else if (line.StartsWith("[BlackElo "))
                {
                    int.TryParse(GetValue(line), out int blackElo);
                    game.BlackElo = blackElo;
                }
                else if (line.StartsWith("[Result "))
                {
                    string result = GetValue(line);
                    if (result == "1-0")
                    {
                        game.Result = 'W';
                    }
                    else if (result == "0-1")
                    {
                        game.Result = 'B';
                    }
                    else
                    {
                        game.Result = 'D';
                    }
                }
                else if (line.StartsWith("[EventDate "))
                {
                    if (GetValue(line).Contains('?'))
                    {
                        game.EventDate = "000-00-00";
                        continue;
                    }
                    game.EventDate = GetValue(line);
                }
                else if (!line.StartsWith("["))
                {
                    moves.Append(line);
                }
            }

            if (!string.IsNullOrWhiteSpace(game.Moves))
            {
                games.Add(game);
            }

            return games;
        }

        // help method to return element between ""
        private static string GetValue(string line)
        {
            return line.Split('"')[1];
        }
    }
}