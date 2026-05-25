using Microsoft.Extensions.Logging;
using Microsoft.Maui.Controls;
using MySqlConnector;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

/*
  Author: Daniel Kopta and ...
  Chess browser backend 
*/

namespace ChessBrowser
{
    internal class Queries
    {

        /// <summary>
        /// This function runs when the upload button is pressed.
        /// Given a filename, parses the PGN file, and uploads
        /// each chess game to the user's database.
        /// </summary>
        /// <param name="PGNfilename">The path to the PGN file</param>
        internal static async Task InsertGameData(string PGNfilename, MainPage mainPage)
        {
            // This will build a connection string to your user's database on atr,
            // assuimg you've typed a user and password in the GUI
            string connection = mainPage.GetConnectionString();

            //       Load and parse the PGN file
            //       We recommend creating separate libraries to represent chess data and load the file
            Debug.WriteLine("start loading file");
            List<ChessGame> games = ParsingPGNFile.LoadPGN(PGNfilename);



            //       Use this to tell the GUI's progress bar how many total work steps there are
            //       For example, one iteration of your main upload loop could be one work step
            //mainPage.SetNumWorkItems( ... );
            Debug.WriteLine("loading file finished, there are : " + games.Count + " game(s) found.");
            mainPage.SetNumWorkItems(games.Count);

            using (MySqlConnection conn = new MySqlConnection(connection))
            {
                try
                {
                    // Open a connection
                    conn.Open();

                    //       iterate through your data and generate appropriate insert commands
                    foreach (ChessGame game in games)
                    {
                        try
                        {
                            // insert / update Player
                            int whitePlayerID = GetOrUpdatePlayerID(game.White, game.WhiteElo, conn);
                            int blackPlayerID = GetOrUpdatePlayerID(game.Black, game.BlackElo, conn);
                            // insert / update Event
                            int eventID = GetOrUpdateEventID(game.Event, game.Site, game.EventDate, conn);

                            // insert Game
                            InsertGame(game, eventID, whitePlayerID, blackPlayerID, conn);
                        }
                        catch (Exception ex)
                        {
                            // server return error, print to bedug port
                            Debug.WriteLine(ex.ToString());
                        }

                        //       Use this inside a loop to tell the GUI that one work step has completed:
                        await mainPage.NotifyWorkItemCompleted();
                    }

                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine(e.Message);
                }

                Debug.WriteLine("Finished");
            }

        }

        // return player id or add / update and return player id
        private static int GetOrUpdatePlayerID(string playerName, int elo, MySqlConnection conn)
        {
            int playerID = GetPlayerID(playerName, conn);
            // -1 id indicate not exist
            if (playerID == -1)
            {
                playerID = InsertPlayer(playerName, elo, conn);
            }
            else
            {
                UpdatePlayerElo(playerID, elo, conn);
            }
            return playerID;
        }

        /// <summary>
        /// get player ID
        /// </summary>
        /// <param name="playerName">Name to search</param>
        /// <param name="conn">connection</param>
        /// <returns>ID we find, or -1 if not find</returns>
        private static int GetPlayerID(string playerName, MySqlConnection conn)
        {
            string query = "SELECT pID FROM Players WHERE Name = @Name";
            MySqlCommand command = new MySqlCommand(query, conn);
            command.Parameters.AddWithValue("@Name", playerName);

            var result = command.ExecuteScalar();
            return result == null ? -1 : Convert.ToInt32(result);
        }

        // when player not exist, unsert player and return id
        private static int InsertPlayer(string playerName, int elo, MySqlConnection conn)
        {
            string query = "INSERT INTO Players (Name, Elo) VALUES (@Name, @Elo)";
            MySqlCommand command = new MySqlCommand(query, conn);
            // attach to variable in query
            command.Parameters.AddWithValue("@Name", playerName);
            command.Parameters.AddWithValue("@Elo", elo);
            command.ExecuteNonQuery();

            return (int)command.LastInsertedId;
        }

        // when player exist, need to update elo
        private static void UpdatePlayerElo(int playerID, int elo, MySqlConnection conn)
        {
            // always keep max Elo
            string query = "UPDATE Players SET Elo = GREATEST(Elo, @Elo) WHERE pID = @PlayerID";
            MySqlCommand command = new MySqlCommand(query, conn);
            command.Parameters.AddWithValue("@Elo", elo);
            command.Parameters.AddWithValue("@PlayerID", playerID);
            command.ExecuteNonQuery();
        }

        // [insert event], return event ID 
        private static int GetOrUpdateEventID(string eventName, string site, string eventDate, MySqlConnection conn)
        {
            int eventID = GetEventID(eventName, site, eventDate, conn);
            if (eventID == -1)
            {
                eventID = InsertEvent(eventName, site, eventDate, conn);
            }
            return eventID;
        }

        // get event id, -1 indicate not exist
        private static int GetEventID(string eventName, string site, string eventDate, MySqlConnection conn)
        {
            string query = "SELECT eID FROM Events WHERE Name = @Name AND Site = @Site AND Date = @Date";
            MySqlCommand command = new MySqlCommand(query, conn);
            command.Parameters.AddWithValue("@Name", eventName);
            command.Parameters.AddWithValue("@Site", site);
            command.Parameters.AddWithValue("@Date", DateTime.Parse(eventDate));

            object? result = command.ExecuteScalar();
            return result == null ? -1 : Convert.ToInt32(result);
        }

        // insert event and return event ID
        private static int InsertEvent(string eventName, string site, string eventDate, MySqlConnection conn)
        {
            string query = "INSERT INTO Events (Name, Site, Date) VALUES (@Name, @Site, @Date)";
            MySqlCommand command = new MySqlCommand(query, conn);
            command.Parameters.AddWithValue("@Name", eventName);
            command.Parameters.AddWithValue("@Site", site);
            command.Parameters.AddWithValue("@Date", DateTime.Parse(eventDate));
            command.ExecuteNonQuery();

            return (int)command.LastInsertedId;
        }

        // insert one game
        private static void InsertGame(ChessGame game, int eventID, int whitePlayerID, int blackPlayerID, MySqlConnection conn)
        {
            string query = "INSERT INTO Games (Round, Result, Moves, BlackPlayer, WhitePlayer, eID) " + "VALUES (@Round, @Result, @Moves, @BlackPlayer, @WhitePlayer, @EventID)";
            MySqlCommand command = new MySqlCommand(query, conn);
            command.Parameters.AddWithValue("@Round", game.Round);
            command.Parameters.AddWithValue("@Result", game.Result);
            command.Parameters.AddWithValue("@Moves", game.Moves);
            command.Parameters.AddWithValue("@BlackPlayer", blackPlayerID);
            command.Parameters.AddWithValue("@WhitePlayer", whitePlayerID);
            command.Parameters.AddWithValue("@EventID", eventID);
            command.ExecuteNonQuery();
        }





        // ********************************* Query Part ********************************* 

        /// <summary>
        /// Queries the database for games that match all the given filters.
        /// The filters are taken from the various controls in the GUI.
        /// </summary>
        /// <param name="white">The white player, or null if none</param>
        /// <param name="black">The black player, or null if none</param>
        /// <param name="opening">The first move, e.g. "1.e4", or null if none</param>
        /// <param name="winner">The winner as "W", "B", "D", or null if none</param>
        /// <param name="useDate">True if the filter includes a date range, False otherwise</param>
        /// <param name="start">The start of the date range</param>
        /// <param name="end">The end of the date range</param>
        /// <param name="showMoves">True if the returned data should include the PGN moves</param>
        /// <returns>A string separated by newlines containing the filtered games</returns>
        internal static string PerformQuery(string white, string black, string opening,
          string winner, bool useDate, DateTime start, DateTime end, bool showMoves,
          MainPage mainPage)
        {
            // This will build a connection string to your user's database on atr,
            // assuimg you've typed a user and password in the GUI
            string connection = mainPage.GetConnectionString();

            // Build up this string containing the results from your query
            string parsedResult = "";

            // Use this to count the number of rows returned by your query
            // (see below return statement)
            int numRows = 0;

            using (MySqlConnection conn = new MySqlConnection(connection))
            {
                try
                {
                    // Open a connection
                    conn.Open();

                    //       Generate and execute an SQL command,
                    //       then parse the results into an appropriate string and return it.
                    //string query = "select Events.Name as Event, Events.Site as Site, Events.Date as EventDate, wt.Name as White, wt.Elo as WhiteElo, bk.Name as Black, bk.Elo as BlackElo, Moves, Games.Result from Events natural join Games join Players as bk join Players as wt where(wt.pID = Games.WhitePlayer and bk.pID = Games.BlackPlayer) and wt.Name = @white";
                    string query = ConstructQuery(white, black, opening, winner, useDate, start, end);
                    MySqlCommand command = new MySqlCommand(query, conn);
                    command.Parameters.AddWithValue("@white", white);
                    command.Parameters.AddWithValue("@black", black);
                    command.Parameters.AddWithValue("@opening", opening);
                    command.Parameters.AddWithValue("@winner", winner);
                    command.Parameters.AddWithValue("@start", start);
                    command.Parameters.AddWithValue("@end", end);

                    using (var reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            parsedResult += FormatGame(reader, showMoves);
                            numRows++;
                        }
                    }
                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine(e.Message);
                }
            }

            return numRows + " results\n\n" + parsedResult;
        }

        // return query to search in DB
        private static string ConstructQuery(string white, string black, string opening, string winner, bool useDate, DateTime start, DateTime end)
        {
            string query = "select Events.Name as Event, Events.Site as Site, Events.Date as EventDate, wt.Name as White, wt.Elo as WhiteElo, bk.Name as Black, bk.Elo as BlackElo, Moves, Games.Result from Events natural join Games join Players as bk join Players as wt where (wt.pID = Games.WhitePlayer and bk.pID = Games.BlackPlayer)";

            // add filter rule
            if (!string.IsNullOrEmpty(white))
                query += $" and wt.Name = @white";
            if (!string.IsNullOrEmpty(black))
                query += $" and bk.Name = @black";
            if (!string.IsNullOrEmpty(opening))
                query += $" and Moves LIKE concat(@opening, '%')";
            if (!string.IsNullOrEmpty(winner))
                query += $" and Result = @winner";
            if (useDate)
                query += $" and Events.Date BETWEEN @start and @end";

            Debug.WriteLine(query);
            return query;

        }

        // format to post in screen
        private static string FormatGame(MySqlDataReader reader, bool showMoves)
        {
            string formattedGame = $"Event: {reader.GetString("Event")}\n" +
                                   $"Site: {reader.GetString("Site")}\n" +
                                   $"Date: {reader.GetDateTime("EventDate")}\n" +
                                   $"White: {reader.GetString("White")} ({reader.GetInt32("WhiteElo")})\n" +
                                   $"Black: {reader.GetString("Black")} ({reader.GetInt32("BlackElo")})\n" +
                                   $"Result: {reader.GetChar("Result")}\n";

            if (showMoves)
                formattedGame += $"Moves: {reader.GetString("Moves")}\n";

            return formattedGame + "\n";
        }

    }
}
