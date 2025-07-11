package com.example.curtainopener

import android.util.Log
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.encodeToJsonElement
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import java.io.IOException
import java.time.Instant
import java.time.LocalDateTime
import java.time.LocalTime
import java.time.ZoneId
import java.time.ZoneOffset
import java.time.format.DateTimeFormatter


@Serializable
data class StatusJson(
    val alarmStatus: Boolean,
    val time: Long,
    val temperature: Double,
    val humidity: Double,
    val openingTimes: MutableList<String>
)

data class StatusUiState(
    val alarmStatus: Boolean = false,
    val time: LocalTime? = LocalTime.MIN,
    val timeCorrect: Boolean = false,
    val temperature: Double = 0.0,
    val humidity: Double = 0.0,
    val openingTimes: Map<String, String> = mapOf<String, String>("" to "")
)

class HttpClientViewModel : ViewModel() {
    private val client = OkHttpClient()
    private val _uiState = MutableStateFlow(StatusUiState())
    val uiState: StateFlow<StatusUiState> = _uiState.asStateFlow()

    companion object {
        val MEDIA_TYPE_JSON = "application/json; charset=utf-8".toMediaType()
        val MEDIA_TYPE_PLAIN = "plain/text; charset=utf-8".toMediaType()
    }

    fun getStatus() {
        viewModelScope.launch(Dispatchers.IO) {
            val request = Request.Builder().url("http://192.168.1.142:8000/status").build()

            try {
                client.newCall(request).execute().use { response ->
                    if (!response.isSuccessful) {
                        Log.e("GET", "Unexpected response: $response")
                        return@use
                    }

                    withContext(Dispatchers.Main) {
                        _uiState.update { currentState ->
                            response.body?.let { it1 ->
                                val status = Json.decodeFromString<StatusJson>(it1.string())
                                val time = Instant.ofEpochSecond(status.time)
                                    .atZone(ZoneId.systemDefault()).toLocalTime()
                                val dtf: DateTimeFormatter = DateTimeFormatter.ofPattern("HH:mm")
                                // probably not ideal, accuracy by minutes is good enough though
                                val timeCorrect = time.format(dtf) == LocalTime.now().format(dtf)
                                val weekdays =
                                    listOf(
                                        "Monday",
                                        "Tuesday",
                                        "Wednesday",
                                        "Thursday",
                                        "Friday",
                                        "Saturday",
                                        "Sunday"
                                    )
                                // sort the list to normal start day
                                val first = status.openingTimes.removeAt(0)
                                status.openingTimes.add(first)
                                val openTimeList = status.openingTimes

                                val openingTimesMap = weekdays.zip(openTimeList).toMap()
                                println(openingTimesMap)
                                currentState.copy(
                                    alarmStatus = status.alarmStatus,
                                    time = time,
                                    timeCorrect = timeCorrect,
                                    temperature = status.temperature,
                                    humidity = status.humidity,
                                    openingTimes = openingTimesMap
                                )
                            }!!
                        }
                    }
                }
            } catch (e: Exception) {
                Log.e("GET", "Exception: ${e.message}", e)
            }
        }
    }

    fun setArduinoTime() {
        viewModelScope.launch(Dispatchers.IO) {
            val offset: ZoneOffset = ZoneId.systemDefault().rules.getOffset(LocalDateTime.now())
            val postBody = LocalDateTime.now().toEpochSecond(offset).toString()

            val request = Request.Builder().url("http://192.168.1.142:8000/set/time")
                .post(postBody.toRequestBody(MEDIA_TYPE_PLAIN)).build()

            try {
                client.newCall(request).execute().use { response ->
                    if (!response.isSuccessful) {
                        Log.e("GET", "Unexpected response: $response")
                        return@use
                    }
                    getStatus()
                }
            } catch (e: IOException) {
                Log.e("Network", "Error: ${e.message}", e)
            }
        }
    }

    fun setOpenTimes(openingTimes: MutableMap<String, String>) {
        val openingTimesList = listOf<String>()
        openingTimes.forEach { day, time ->

        }

        println(openingTimesList)
        val postBody =
            Json.encodeToString("openingTimes: ${Json.encodeToJsonElement(openingTimesList)}")
        println(postBody)

        viewModelScope.launch(Dispatchers.IO) {
            val request = Request.Builder().url("http://192.168.1.142:8000/set/open")
                .post(postBody.toRequestBody(MEDIA_TYPE_JSON)).build()

            client.newCall(request).execute().use { response ->
                if (!response.isSuccessful) {
                    Log.e("GET", "Unexpected response: $response")
                    return@use
                }
                getStatus()
            }
        }
    }

    fun toggleAlarm() {
        viewModelScope.launch(Dispatchers.IO) {
            val request = Request.Builder().url("http://192.168.1.142:8000/alarm").build()

            try {
                client.newCall(request).execute().use { response ->
                    if (!response.isSuccessful) {
                        Log.e("GET", "Unexpected response: $response")
                        return@use
                    }

                    withContext(Dispatchers.Main) {
                        _uiState.update { currentState ->
                            response.body?.let { it1 ->
                                val alarm = it1.string().toBoolean()
                                currentState.copy(
                                    alarmStatus = alarm
                                )
                            }!!
                        }
                    }
                }
            } catch (e: Exception) {
                Log.e("GET", "Exception: ${e.message}", e)
            }
        }
    }
}
