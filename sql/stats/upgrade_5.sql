/* DB Version 5 to Version 6 */
ALTER TABLE game_players ADD COLUMN public_key TEXT DEFAULT NULL;
